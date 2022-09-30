/*
    SDImageGenerator, Text to image generation AI app
    Copyright(C) 2022 Rupesh Sreeraman
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "diffusionenvvalidator.h"

DiffusionEnvValidator::DiffusionEnvValidator(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    diffusionEnv = diffEnv;
    pipValidator = new PythonEnvValidator(parent,diffEnv);
    connect(pipValidator,SIGNAL(packageValidationCompleted(int,bool)),this,SLOT(packageValidationCompleted(int,bool)));

}

void DiffusionEnvValidator::Validate()
{
    pipValidator->validatePackages();
}

bool DiffusionEnvValidator::validateModelPath()
{
    if( Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath())) {
        return validateModelFile();
    }
    return false;
}

bool DiffusionEnvValidator::validateModelFileSize()
{
    return validateModelFile();
}

void DiffusionEnvValidator::packageValidationCompleted(int , bool isPackagesReady)
{
    bool modelReady = validateModelPath();
    emit environmentCurrentStatus(isPackagesReady, modelReady);

}

bool DiffusionEnvValidator::validateModelFile()
{
    QFile modelFile(diffusionEnv->getStableDiffusionModelPath());
    return STABLE_DIFFUSION_MODEL_1_4_FILE_SIZE == modelFile.size();
}
