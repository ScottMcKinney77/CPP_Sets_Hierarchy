/*
    Code for project 6, CSC 254, Fall 2019

    DO NOT USE FOR ACADEMIC DISHONESTY

    Contains Hierarchy of sets:
        -Simple Set
        -Simple Characteristic Array set
        -Simple Hashed set
        -Simple BinSearch set

        -Ranged Set
        -Ranged Characteristic Array set
        -Ranged Hashed set
        -Ranged BinSearch set
    All sets implemented using only basic arrays
*/

#include <string.h>

using namespace std;

// Naive comparator.
// Provides a default for any type that has an operator<
// and an operator==.
//
template<typename T>
class comp {
public:
    bool precedes(const T& a, const T& b) const {
        if (a < b){
            (void) a;
            (void) b;
            return true;
        }else{
            (void) a;
            (void) b;
            return false;
        }
    }
    bool equals(const T& a, const T& b) const {
        if (a == b){
            (void) a;
            (void) b;
            return true;
        }else{
            (void) a;
            (void) b;
            return false;
        }
    }
};

// Abstract base class from which all sets are derived.
template<typename T, typename C = comp<T>>
class simple_set {
public:
    virtual ~simple_set<T, C>() { }
    virtual simple_set<T, C>& operator+=(const T item) = 0;
    virtual simple_set<T, C>& operator-=(const T item) = 0;
    virtual bool contains(const T& item) const = 0;
};

//---------------------------------------------------------------

// Characteristic array implementation of set.
// Requires instantiation with guaranteed low and one-more-than-high
// bounds on elements that can be placed in the set.  Should compile
// and run correctly for any element class T that can be cast to int.
// Throws out_of_bounds exception when appropriate.
//
class out_of_bounds { };    // exception
template<typename T>
class carray_simple_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
private:
    int low, high;
    T* arrPoint;
public:
    carray_simple_set(const T l, const T h) {   // constructor
        low = static_cast<int>(l);
        high = static_cast<int>(h);
        T* arr = new T[high-low];
        arrPoint = arr;
    }
    virtual carray_simple_set<T>& operator+=(const T item2) {
        this->arrPoint[static_cast<int>(item2) - this->low] = item2;
        return *this;
    }
    virtual carray_simple_set<T>& operator-=(const T item) {
        this->arrPoint[static_cast<int>(item) - this->low] = {};
        (void) item;  return *this;
    }
    virtual bool contains(const T& item) const {
        if (this->arrPoint[static_cast<int>(item) - this->low] == item){
            return true;
        }else{
            return false;
        }
    }
    virtual int getL(){
        return this->low;
    }
    virtual int getH(){
        return this->high;
    }
    virtual T* getArrPoint(){
        return this->arrPoint;
    }
};
//---------------------------------------------------------------

// Naive hash function object.
// Provides a default for any type that can be cast to int.
//
template<typename T>
class cast_to_int {
public:
    int operator()(const T n) {
        return (int) n;
    }
};

// Hash table implementation of set.
// Requires instantiation with guaranteed upper bound on number of elements
// that may be placed in set.  Throws overflow if bound is exceeded.
// Can be instantiated without second generic parameter if element type
// can be cast to int; otherwise requires hash function object.
//
template<typename T>
class HashNode{
public:
    T value;
    int key;
    HashNode (int key, T value){
        this->value = value;
        this->key = key;
    }
};

class overflow { };         // exception
template<typename T, typename F = cast_to_int<T>>
class hashed_simple_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
private:
    HashNode<T> **arr;
    int maxNumElems;
    int curSize;
    T* arrPoint;
    HashNode<T> *dummy;
public:
    hashed_simple_set(const int n) {    // constructor
        maxNumElems = n;
        curSize = 0;
        arr = new HashNode<T>*[maxNumElems];
        for (int i = 0; i < maxNumElems; i++){
            arr[i] = NULL;
        }
        F caster4;

        dummy = new HashNode<T>(-1,-1);
        (void) n;
    }
    int hashCode(int key) const{
        return key % maxNumElems;
    }
    virtual hashed_simple_set<T, F>& operator+=(const T item) {
        F caster;
        HashNode<T> *temp = new HashNode<T>(caster(item),item);

        int hashIndex = hashCode(caster(item));

        while(arr[hashIndex] != NULL && arr[hashIndex]->key != caster(item) && arr[hashIndex]->key != -1){
            hashIndex++;
            hashIndex %= maxNumElems;
        }
        if(arr[hashIndex] == NULL || arr[hashIndex]->key == -1){
            curSize++;
        }
        arr[hashIndex] = temp;
        return *this;
    }
    virtual hashed_simple_set<T, F>& operator-=(const T item) {
        F caster2;
        int hashIndex = hashCode(caster2(item));

        while (arr[hashIndex] != NULL){
            if(arr[hashIndex]->key == caster2(item)){
                HashNode<T> *temp = arr[hashIndex];
                arr[hashIndex] = dummy;
                curSize--;
                return *this;
            }
            hashIndex++;
            hashIndex %= maxNumElems;
        }
        //NOT FOUND
        return *this;
    }
    virtual bool contains(const T& item) const {
        F caster3;
        int hashIndex = hashCode(caster3(item));
        int counter = 0;
        while(arr[hashIndex] != NULL){
            if (counter++>maxNumElems){
                return false;
            }
            if (arr[hashIndex]->key == caster3(item)){
                return true;
            }
            hashIndex++;
            hashIndex %= maxNumElems;
        }
        return false; //To make the compiler happy
    }
};
//---------------------------------------------------------------

// Sorted array implementation of set; supports binary search.
// Requires instantiation with guaranteed upper bound on number of
// elements that may be placed in set.  Throws overflow if bound is
// exceeded.
template<typename T>
class BinNode{
public:
    T value;
    BinNode<T> *left, *right;
    BinNode (T value){
        this->value = value;
        this->left = NULL;
        this->right = NULL;
    }
};


template<typename T, typename C = comp<T>>
class bin_search_simple_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
public:
    BinNode<T> *root;
    C* comparator;
    int maxNumElems;
    bin_search_simple_set(const int n) {    // constructor
        maxNumElems = n;
        this->root = NULL;
        (void) n;
    }
    virtual bin_search_simple_set<T, C>& operator+=(const T item) {
        BinNode<T>* temp = new BinNode<T>(item);
        if (this->root == NULL){
            root = temp;
            return *this;
        }
        BinNode<T>* x = root;
        BinNode<T>* y = NULL;

        while (x != NULL){
            y = x;
            if (comparator->precedes(temp->value, x->value)){
                x = x->left;
            }else{
                x = x->right;
            }
        }
        if (y == NULL){
            y = temp;
        }else if (comparator->precedes(temp->value,y->value)){
            y->left = temp;
        }else{
            y->right = temp;
        }
        return *this;
    }
    virtual bin_search_simple_set<T, C>& operator-=(const T item) {
        BinNode<T>* temp = new BinNode<T>(item);
        BinNode<T>* root = this->root;
        root = recDelete(root, temp);
        this->root = root;
        return *this;
    }
    BinNode<T>* recDelete(BinNode<T>* root, BinNode<T>* temp){
        if (root == NULL){
            return root;
        }
        if (comparator->equals(temp->value, root->value)){
            if (root->right == NULL){
                BinNode<T> *temp2 = root->left;
                delete root;
                return temp2;
            }else if(root->left == NULL){
                BinNode<T> *temp2 = root->right;
                delete root;
                return temp2;
            }
            BinNode<T> *temp2 = minNode(root->right);
            root->value = temp2->value;

            root->right = recDelete (root->right,temp2);
        }else if (comparator->precedes(temp->value, root->value)){
            root->left = recDelete(root->left,temp);
        }else{
            root->right = recDelete(root->right,temp);
        }
        return root;
    }
    BinNode<T>* minNode(BinNode<T>* node){
        BinNode<T>* temp = node;
        while (temp && temp->left != NULL){
            temp = temp->left;
        }
        return temp;
    }
    virtual bool contains(const T& item) const{
        BinNode<T>* temp = new BinNode<T>(item);
        BinNode<T>* x = root;
        while (x != NULL){
            if (comparator->equals(temp->value, x->value)){
                return true;
            }else if (comparator->precedes(temp->value, x->value)){
                x = x->left;
            }else{
                x = x->right;
            }
        }
        return false;
    }
};
//===============================================================
// RANGE SETS

// Function object for incrementing.
// Provides a default for any integral type.
//
template<typename T>
class increment {
    static_assert(std::is_integral<T>::value, "Integral type required.");
public:
    T operator()(T a) const {
        return ++a;
    }
};

static const struct { } empty_range;    // exception
template<typename T, typename C = comp<T>>
class range {
    T L;        // represents all elements from L
    bool Linc;  // inclusive?
    T H;        // through H
    bool Hinc;  // inclusive?
    C cmp;      // can't be static; needs explicit instantiation
public:
    range(const T l, const bool linc, const T h, const bool hinc)
    : L(l), Linc(linc), H(h), Hinc(hinc), cmp() {
        if (cmp.precedes(h, l)
            || (cmp.equals(l, h) && (!Linc || !Hinc))) throw empty_range;
    }
    T low() const { return L; }
    bool closed_low() const { return Linc; }
    T high() const { return H; }
    bool closed_high() const {return Hinc; }
    bool contains(const T& item) const {
        return ((cmp.precedes(L, item) || (Linc && cmp.equals(L, item)))
            && (cmp.precedes(item, H) || (Hinc && cmp.equals(item, H))));
    }
};

// This is the abstract class from which all range-supporting sets are derived.
//
template<typename T, typename C = comp<T>>
class range_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
public:
    virtual range_set<T, C>& operator+=(const range<T, C> r) = 0;
    virtual range_set<T, C>& operator-=(const range<T, C> r) = 0;
};

//---------------------------------------------------------------

//Carray ranged set
template<typename T, typename C = comp<T>, typename I = increment<T>>
class carray_range_set : public virtual range_set<T,C>, public carray_simple_set<T> {
    static_assert(std::is_integral<T>::value, "Integral type required.");
    I inc;
public:
    carray_range_set(const T l, const T h) : range_set<T,C>(), carray_simple_set<T>(l,h){}

    virtual carray_simple_set<T>& operator+=(const T item) {
        return carray_simple_set<T>::operator+=(item);
    }
    virtual carray_simple_set<T>& operator-=(const T item) {
        return carray_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return carray_simple_set<T>::contains(item);
    }
    virtual range_set<T>& operator+=(const range<T, C> r){
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
            r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
            r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

//Hashed Ranged Set:

template<typename T, typename F = cast_to_int<T>, typename C = comp<T>, typename I = increment<T>>
class hashed_range_set : public virtual range_set<T,C>, public hashed_simple_set<T,F> {
    static_assert(std::is_integral<T>::value, "Integral type required. ");
    I inc;
public:
    hashed_range_set(const int n) : range_set<T,C>(), hashed_simple_set<T,F>(n){
    }
    virtual hashed_simple_set<T,F>& operator+=(const T item) {
        return hashed_simple_set<T,F>::operator+=(item);
    }
    virtual hashed_simple_set<T,F>& operator-=(const T item) {
        return hashed_simple_set<T,F>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return hashed_simple_set<T,F>::contains(item);
    }
    virtual range_set<T>& operator+=(const range<T, C> r){
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
            r.contains(i); i = inc(i)) {
                *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
            r.contains(i); i = inc(i)) {
                *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

//Bin Search Ranged Set : Not done yet

template<typename T, typename C = comp<T>>
class bin_search_range_set : public virtual range_set<T,C>, public bin_search_simple_set<T,C> {
public:
    bin_search_range_set(const int n) : range_set<T,C>(), bin_search_simple_set<T,C>(n){
    }
    virtual bin_search_simple_set<T,C>& operator+=(const T item) {
        return bin_search_simple_set<T,C>::operator+=(item);
    }
    virtual bin_search_simple_set<T,C>& operator-=(const T item) {
        return bin_search_simple_set<T,C>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return bin_search_simple_set<T,C>::contains(item);
    }
    virtual range_set<T>& operator+=(const range<T, C> r){
        return *this;
    }
    virtual range_set<T>& operator-=(const range<T, C> r) {
        return *this;
    }

};
