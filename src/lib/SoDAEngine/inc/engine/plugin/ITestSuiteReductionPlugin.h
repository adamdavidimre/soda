/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IREDUCTIONPLUGIN_H
#define IREDUCTIONPLUGIN_H

#include <iostream>
#include <fstream>

#include "data/CSelectionData.h"
#include "io/CJsonReader.h"

using namespace soda::io;

namespace soda {

/**
 * @brief The IReductionPlugin class is an interface class for reduction plugins.
 */
class ITestSuiteReductionPlugin
{
public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~ITestSuiteReductionPlugin() {}

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual String getName() = 0;

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual String getDescription() = 0;

    /**
     * @brief Fills the reduction plugin with data.
     */
    virtual void init(CSelectionData *data, CJsonReader &reader) = 0;

    /**
     * @brief Runs the reduction algorithm.
     * @param outStream Output stream.
     */
    virtual void reduction(std::ofstream &outStream) = 0;
};

} /* namespace soda */

#endif /* IREDUCTIONPLUGIN_H */
