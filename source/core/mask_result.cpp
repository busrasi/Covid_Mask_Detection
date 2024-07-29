#include "mask_result.h"

MaskResult::~MaskResult()
{}

void MaskResult::SetResult(bool val)
{
    m_Result = val;
}

bool MaskResult::GetResult() const
{
    return m_Result;
}
