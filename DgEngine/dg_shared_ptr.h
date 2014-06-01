#ifndef DG_SHARED_PTR
#define DG_SHARED_PTR

namespace impl
{
  class RefCounter
  {
  private:
    unsigned int count;

  public:
    RefCounter() : count(0) {}
    void AddRef() {count++;}
    int Release() {return --count;}
  };
}

namespace Dg
{
  template < typename T > 
  class shared_ptr
  {
  public:
    shared_ptr() : pData(0), reference()
    {
      reference = new RefCounter();
      reference->AddRef();
    }

    shared_ptr(T* pValue) : pData(pValue), reference()
    {
      reference = new RefCounter();
      reference->AddRef();
    }

    shared_ptr(const shared_ptr<T>& sp) : pData(sp.pData), reference(sp.reference)
    {
      reference->AddRef();
    }

    ~shared_ptr()
    {
      if (reference->Release() == 0)
      {
        delete pData;
        delete reference;
      }
    }

    T& operator* ()
    {
      return *pData;
    }

    T* operator-> ()
    {
      return pData;
    }

    shared_ptr<T>& operator = (const shared_ptr<T>& sp)
    {
      if (this != &sp) // Avoid self assignment
      {
        // Decrement the old reference count
        // if reference become zero delete the old data
        if (reference->Release() == 0)
        {
          delete pData;
          delete reference;
        }

        // Copy the data and reference pointer
        // and increment the reference count
        pData = sp.pData;
        reference = sp.reference;
        reference->AddRef();
      }
      return *this;
    }

  private:
    T*          pData;       // pointer
    RefCounter* reference;   // Reference count
  };
}
#endif