/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#ifndef IRESULTSREADERPLUGIN_H
#define IRESULTSREADERPLUGIN_H

#include "data/CResultsMatrix.h"

namespace soda {

/**
 * @brief Interface of the results reader plugins.
 */
class IResultsReaderPlugin
{
public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~IResultsReaderPlugin() {}
    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual std::string getName() = 0;
    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual std::string getDescription() = 0;

    /**
     * @brief Reads results from the specified path.
     * @param [in] path The path to the results.
     * @return A CResultsMatrix instance.
     */
    virtual CResultsMatrix* read(const std::string &path) = 0;
};

} /* namespace soda */

#endif /* IRESULTSREADERPLUGIN_H */
