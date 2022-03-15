#pragma ones
/**
 * @brief 并查集数据结构的实现，采用结构体
 * 
 */
//
typedef struct 
{
    int data;      // 集合的数据类型
    int rank;      // 集合所处的层次，通常初始为0
    int parent;    // 并查集的代表元（表示集合的类别）
}universe_element;

class Universe
{
private:
    universe_element* uni_ele;
    int num;      
public:
    universe(const int elements);
    ~universe();

    int find(const int x);
    void join(const int x,cons int y);
    int size(const int x) const {return universe_element[x].data}
    int num_sets const {return num;}
}
universe::universe(int elements)
{
    uni_ele = new universe_element[elements];
    num = elements;
    for (int i = 0; i < elements; i++)
    {
        uni_ele[i].rank = 0;
        uni_ele[i].data = 1;
        uni_ele[i].parent = i;
    }
}

universe::~universe()
{
    delete [] uni_ele;
}

int universe::find(const int x)
{
    int step = x;
    while(step != uni_ele[step].parent)
    {
        step = uni_ele[step].parent;
    }
    uni_ele[x].parent = step;

    return step;
}

void universe::jon(const int x,const int y)
{
    if (uni_ele[x].rank > uni_ele[y].rank)
    {
        uni_ele[y].parent = x;
        uni_ele[x].data += uni_ele[y].data;
    }
    else
    {
        uni_ele[x].parent = y;
        uni_ele[y].data += uni_ele[x].data;
        if (uni_ele[x].rank == uni_ele[y].rank)
        {
            uni_ele[y].rank++;
        }
    }
    num--;
}