#include "DVRTextureGenerate.h"

#include <ostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

static std::ifstream file;

#if defined(_WIN32)
static const char petFile[] = ".\\res\\pet_raw.bytes";
static const char transFile[] = ".\\res\\transfer.data";
#else
static const char petFile[] = "./res/pet_raw.bytes";
static const char transFile[] = "./res/transfer.data";
#endif

static const int DATAX = 200;
static const int DATAY = 160;
static const int DATAZ = 160;

static GLfloat textureData[DATAX * DATAY * DATAZ];
static GLfloat textureData2[DATAX * DATAY * DATAZ * 4];

GLuint buildTexture3d()
{
    file.open(petFile, std::ofstream::binary);

    file.read((char *)textureData, DATAX * DATAY * DATAZ * sizeof(float));

    file.close();

    for (int k = 0; k < DATAZ; ++k)
    {
        for (int j = 0; j < DATAY; ++j)
        {
            for (int i = 0; i < DATAX; ++i)
            {
                int idx = DATAY * DATAZ * i + DATAZ * j + k;

                float *p = (float *)(&textureData[idx]);

                *p = log(*p);
                *p = *p / 15;
                *p = std::max(*p, 0.0f);
                *p = std::min(*p, 1.0f);

                int idx2 = i + DATAX * j + DATAX * DATAY * k;
                textureData2[idx2 * 4] = *p;
                textureData2[idx2 * 4 + 1] = *p;
                textureData2[idx2 * 4 + 2] = *p;
                textureData2[idx2 * 4 + 3] = *p;
            }
        }
    }

    GLuint texid;

    glGenTextures(1, &texid);

    glBindTexture(GL_TEXTURE_3D,texid);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA32F, DATAX, DATAY, DATAZ);

    glTexSubImage3D(GL_TEXTURE_3D,
                   0,
                   0, 0, 0,
                   DATAX, DATAY, DATAZ,
                   GL_RGBA, GL_FLOAT,
                   textureData2);

    glBindTexture(GL_TEXTURE_3D,0);

    return texid;
}

static const int transferLength = 256;
static GLfloat transferData[transferLength * 4];

GLuint buildTransferFunction()
{
    file.open(transFile, std::ofstream::binary);

    file.read((char *)transferData, transferLength * 4 * sizeof(float));

    file.close();

    GLuint texid;

    glGenTextures(1, &texid);

    glBindTexture(GL_TEXTURE_1D,texid);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F, transferLength);

    glTexSubImage1D(GL_TEXTURE_1D,
                   0,
                   0,
                   transferLength,
                   GL_RGBA, GL_FLOAT, transferData);

    glBindTexture(GL_TEXTURE_1D,0);

    return texid;
}


