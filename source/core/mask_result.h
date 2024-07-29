#ifndef MASK_RESULT_H
#define MASK_RESULT_H


/**
     * \class MaskResult
     *
     * \brief MaskResult Class
     *
     *
     */
class MaskResult
{
public:
    /**
      * @brief C-tor
      */
    explicit MaskResult() = default;

    /**
      * @brief D-tor
      */
    virtual ~MaskResult();

    /**
      * @brief Sets current result.
      * @param val : bool
      */
    void SetResult(bool val);

    /**
      * @brief Gets current result.
      * @return return : bool
      */
    bool GetResult() const;

private:
    bool m_Result = false;
};



#endif // MASK_RESULT_H
