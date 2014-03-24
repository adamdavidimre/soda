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
#ifndef CPARTITIONALGORITHM_H
#define CPARTITIONALGORITHM_H

#include "data/CSelectionData.h"
#include "data/SoDALibDefs.h"

#include "CClusterDefinition.h"

namespace soda {

class CPartitionAlgorithm
{
public:
    /**
      * @brief A code element id and its partition identifier.
      */
    typedef struct {
        IndexType cid;
        IndexType partitionId;
    } partition_info;

    /**
     * @brief Key is revision, value is the list of code elements with their partition identifier.
     */
    typedef std::map<IndexType, std::vector<partition_info> > PartitionInfo;
    /**
     * @brief Key is a partition identifer and the value is a set of code elements belong to the partition.
     */
    typedef std::map<IndexType, std::set<IndexType> > PartitionData;
    /**
     * @brief Key is revision, value is PartitionData.
     */
    typedef std::map<IndexType, PartitionData > Partition;

private:
    typedef struct {
        IndexType sum;
        IndexType cid;
    } code_element_info;

    friend bool operator<(code_element_info c1, code_element_info c2);

public:
    CPartitionAlgorithm();
    ~CPartitionAlgorithm();

    /**
     * @brief Create the partitions of the test suite for every given revisions.
     * @param [IN] data The input data.
     * @param [IN] cluster The clusters of test cases and code elements.
     * @param [IN] revisionList The revisions to consider.
     */
    void compute(CSelectionData &data, CClusterDefinition &cluster, const std::vector<IndexType> &revisionList);

    /**
     * @brief Create the partitions of the test suite for one revision.
     * @param [IN] data The input data.
     * @param [IN] cluster The clusters of test cases and code elements.
     * @param [IN] revision The revision to consider.
     */
    void compute(CSelectionData &data, CClusterDefinition &cluster, IndexType revision);

    /**
     * @brief Returns the info for every code element.
     * @return
     */
    inline PartitionInfo& getPartitionInfo() { return *m_partitionInfo; }

    /**
     * @brief Returns the data of partitions.
     * @return
     */
    inline Partition& getPartitions() { return *m_partitions; }
private:
    PartitionInfo *m_partitionInfo;
    Partition     *m_partitions;

};

} /* namespace soda */

#endif /* CPARTITIONALGORITHM_H */