#ifndef LAURENT_FINAL_PROJECT_SMARTPOINTER_H
#define LAURENT_FINAL_PROJECT_SMARTPOINTER_H

template <class TYPE>
class SmartPointer {
    TYPE * ptr;
public:
    SmartPointer(TYPE* ptr_ = nullptr):ptr(ptr_){}
    ~SmartPointer(){
        delete ptr;
    }
    TYPE& operator*(){return *ptr;}
    TYPE* operator->() {return ptr;}

};

#endif //LAURENT_FINAL_PROJECT_SMARTPOINTER_H
