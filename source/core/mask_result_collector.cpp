#include "mask_result_collector.h"

#include <cmath>

MaskResultCollector::MaskResultCollector()
{}

MaskResultCollector::~MaskResultCollector()
{}

void MaskResultCollector::addNewResult(bool newResult)
{
    m_maskResult.SetResult(newResult);
}

MaskResult MaskResultCollector::GetResult() const
{
    return m_maskResult;
}
