#ifndef DORI_CORE_SETTINGS_H
#define DORI_CORE_SETTINGS_H

#include "Logic.h"

namespace Dori {
namespace Core {

class Settings
{
private:
    Settings();

public:
    static Settings &getInstance();
    const Logic::Parameters defaultParameters();

private:
    Logic::Parameters mDefaultParameters;
};

} // namespace Core
} // namespace Dori

#endif // DORI_CORE_SETTINGS_H
