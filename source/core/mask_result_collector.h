#ifndef MASK_RESULT_COLLECTOR_H
#define MASK_RESULT_COLLECTOR_H

#include "mask_result.h"

/**
  * \class MaskResultCollector
  *
  * \brief MaskResultCollector
  *
  */
class MaskResultCollector
{
public:
    /**
      * @brief C-tor
      * @param cacheSize : Collection cache size
      */
    explicit MaskResultCollector();

    /**
      * @brief D-tor
      */
    virtual ~MaskResultCollector();

    /**
      * @brief addNewResult : Adding new result depending on criteria
      * @param newResult : bool
      */
    void addNewResult(bool newResult);

    MaskResult GetResult() const;

private:
    MaskResult m_maskResult;
};

#endif // MASK_RESULT_COLLECTOR_H
