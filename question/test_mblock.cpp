#include <iostream>
#include <time.h>

using namespace std;

class A
{
public:
  A(int size = 1000):
    num(size),
    date(NULL)
  {

  }
private:
  int num ;
  A *date;
};

class a_pool
{
public:
  a_pool()
  {
    this->free_list = new A;
  }
  A *alloc()
  {
    return this->free_list;
  }
  void release(A *m)
  {
    this->free_list = m;
  }
private:
  A *free_list;
};

class m_block
{
public:
  m_block(int size = 1024):
    size_(size)
  {
  }

  void init()
  { }
  void reset(void)
  {
    this->next_ = NULL;
  }
  void next(m_block *next)
  {
    this->next_ = next;
  }
  m_block* next()
  {
    return this->next_;
  }
public:
  static void* operator new(size_t size)
  {
    if (size != sizeof(m_block))
      std::cout << "operator new error" << std::endl;
    if (m_block::free_list_ == NULL)
    {
      m_block *mb = m_block::all_mem_.allocate(m_block::chunk_);
      for (int i = 0; i < m_block::chunk_; ++i)
      {
        mb[i].init();
        m_block::add_to_free_list(&mb[i]);
      }
    }
    m_block *head = m_block::free_list_;
    //cout << "ssss" <<  head->next() << endl;
    m_block::free_list_ = head->next();
    return (void *)head;
  }

  static void operator delete(void *point) 
  {
    cout << "come " << endl;
    m_block *temp = (m_block *)point;
    temp->reset();

    m_block::add_to_free_list(temp);
  }
  static int size()
  {
    int i = 0;
    m_block *mb =  m_block::free_list_;
    while(mb != NULL)
    {
      ++i;
      mb = mb->next();
    }
    cout << i << endl;
  }

private:
  static void add_to_free_list(m_block *mb);
  static m_block *free_list_;
  static std::allocator<m_block> all_mem_;
  static int chunk_;
private:
  int size_;
  char *data_;
  int r_ptr_;
  int w_ptr_;
  m_block *next_;
};

void m_block::add_to_free_list(m_block *mb)
{
  mb->next(m_block::free_list_);
  m_block::free_list_ = mb;
}

m_block*  m_block::free_list_ = NULL;
std::allocator<m_block> m_block::all_mem_;
int m_block::chunk_ = 10;

int main(int argc, char *argv[])
{
  int num = ::atoi(argv[1]);
  int begin = clock();
  for (int i = 0; i < num; ++i)
  {
    m_block *k = ::new m_block;
  }
  int end = clock();
  cout << (end - begin)<< endl;
/*
  begin = clock();
  for (int i = 0; i < num; ++i)
  {
    m_block  *k = new m_block;
    delete k;
  }
  end = clock();
  cout << (end - begin)<< endl;
  //m_block::size();
 */

  m_block::size();
  begin = clock();
  for (int i = 0; i < num; ++i)
  {
    m_block *mb = new m_block;
    //cout << mb->next() << endl;
    delete mb;
  }
  end = clock();
  cout << (end - begin)<< endl;
  m_block::size();

  /*
  begin = clock();
  for (int i = 0; i < num; ++i)
  {
    m_block *mb = new m_block;
  }
  end = clock();
  cout << (end - begin)<< endl;
  */
}
