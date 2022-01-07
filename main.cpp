#include <stdio.h>
#include <stdlib.h>
#include <future>
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <map>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <functional>
#include <stdarg.h>
#include <variant>
#include <memory>
#include <concepts>
#include <tuple>
#include <random>
#include <fstream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
using namespace std;

template <size_t n, typename... T>
constexpr variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
    if constexpr (n >= sizeof...(T))
        throw std::out_of_range(" 越界.");
    if (i == n)
        return variant<T...>{ std::in_place_index<n>, std::get<n>(tpl) };
    return _tuple_index<(n < sizeof...(T)-1 ? n+1 : 0)>(tpl, i); 
}

template <typename... T>
constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl, size_t i) {
    return _tuple_index<0>(tpl, i); 
}

template <typename T0, typename ... Ts>
std::ostream & operator<< (std::ostream & s, std::variant<T0, Ts...> const & v) {
    std::visit([&](auto && x){ s << x;}, v);
    return s; 
}

template<typename T>
concept Integral = std::is_integral<T>::value;
void test(Integral auto a, Integral auto b){
    printf("a + b = %d \n", a + b);
}

template<class C, class... Args>
C getC(Args...args){

}

bool writeCSV(){
    ofstream outFile;
    outFile.open("/Users/fangzehua/project/temp/test.csv", ios::app);
    if(!outFile.is_open()){
        printf("outFile Open error \n");
        return false;
    }
    string str;
    int salary = 0, bonus = 0;
    srand((unsigned)time(0));
    for(int i = 0; i < 10000000; i++){
        int r = 0; 
        for(int j = 0; j < 4; j++){
            r = rand() % 26;
            str[j] = 'a' + r;
        }
        salary = rand() % 101;
        bonus = rand() % 6;
        outFile << str.c_str() << "," << salary << "," << bonus  <<  endl;
    }
    
    return true;
}

struct Data
{
    int totalSalary;
    int counts;
    Data(){
        totalSalary = counts = 0;
    }
};


void readCSV(){
    int begin = clock();
    map<string, Data> m;
    std::ifstream inFile("/Users/fangzehua/project/temp/test.csv", std::ios::in);
    vector<string> mList;
    std::string lineStr;
    while(getline(inFile, lineStr))
    {
        mList.clear();
        stringstream ss(lineStr);
        string tmp;
        while(getline(ss, tmp, ','))
            mList.push_back(tmp);
        string name = mList[0].substr(0,2);
        auto iter = m.find(name);
        if(iter == m.end()){
            Data d;
            d.totalSalary = atoi(mList[1].c_str()) * 13 + atoi(mList[2].c_str());
            d.counts = 1;
            m.insert(make_pair(name, d));
        }
        else {
            iter->second.totalSalary += atoi(mList[1].c_str()) * 13 + atoi(mList[2].c_str());
            iter->second.counts++;
        }
    }
    for(auto iter : m){
        printf("姓名[%s], 工资[%d], 人数[%d] \n", iter.first.c_str(), iter.second.totalSalary, iter.second.counts);
    }
    int end  = clock();
    printf(" end - begin : [%d] \n", end - begin);
}

static int counts = 0;

struct node
{
    string data;
    node *p_rNode, *p_lNode;
    node(){
        p_rNode = p_lNode = nullptr;
    }
};

void create_tree(node* &T)
{
    string c;
    cout << "请输入需要插入的字符串" << endl;
	cin >> c;
	if(c.find("null") != string::npos || c.find("NULL") != string::npos)
    {
        return;
    }
		
    printf("counts : [%d], input : [%s] \n", counts++, c.c_str());
	T=new node;
	T->data=c;
	T->p_rNode=NULL;
	T->p_lNode=NULL;
	create_tree(T->p_lNode);
	create_tree(T->p_rNode);
}

void prior_order1(node* T)//递归先序遍历二叉树
{
	if(T)
	{
		printf("%s ", T->data.c_str());
		prior_order1(T->p_lNode);
		prior_order1(T->p_rNode);
	}
}

void prior_order2(node* T)//递归先序遍历二叉树
{
	if(T)
	{
		prior_order1(T->p_lNode);
		printf("%s ", T->data.c_str());
		prior_order1(T->p_rNode);
	}
}

void prior_order3(node* T)//递归先序遍历二叉树
{
	if(T)
	{
		prior_order1(T->p_lNode);
		prior_order1(T->p_rNode);
		printf("%s ", T->data.c_str());
	}
}

int get_now_time(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

typedef struct Cache__
{
    char value[256];
    uint64_t expire_time;
    uint64_t create_time;
}CACHE;

map<int, CACHE> MyCache;

bool check_cache_exist_and_insert(int key, char* msg = nullptr){
    if(key < 0)
    {
        printf("it is not goof key \n");
        return false;
    }

    auto iter = MyCache.find(key);
    if(iter != MyCache.end())
    {
        const auto c = iter->second;
        uint64_t now_time = get_now_time();
        if(now_time - c.create_time > c.expire_time)
        {
            printf("expired \n");
            return false;
        }
        return true;
    }
    else{
        CACHE c;
        sprintf(c.value, "%s", msg);
        c.create_time = get_now_time();
        c.expire_time = (rand()%20) * 1000000;
        MyCache.insert(make_pair(key, c));
        printf("add cache \n");
        return true;
    }
}


int main(){ 
    if(!check_cache_exist_and_insert(1, "hello_wrold"))
        printf("error - 1");
    if(!check_cache_exist_and_insert(2, "o ha yo"))
        printf("error - 2");
    if(!check_cache_exist_and_insert(3, "yo lo xi ku"))
        printf("error - 3");
    if(!check_cache_exist_and_insert(4, " 你好"))
        printf("error - 4");
    
    sleep(20);
    if(!check_cache_exist_and_insert(1))
        printf("error - 5");
}

// int main(){
//     //writeCSV();
//     readCSV();
//     // map<tuple<int, float, double, string>, string> t_map;
//     // t_map.insert(make_pair(make_tuple(1,2.0,3.0,"hello"), "world"));
//     // printf("%s \n", t_map[make_tuple(1,2.0,3.0,"hello")].c_str()); 
//     // thread t([](int a, int b, int c){sleep(10); printf("a = [%d], b = [%d], c = [%d] \n", a, b, c);}, 2000, 3000, 4000);
//     // t.join();
//     // test(20, 30);
// }

/*
int main(){
    // tuple<string, double, int, char> t = make_tuple("hello wrold!", 20.01, 30, 'h');
    // cout << tuple_index(t, 0) << endl;
    shared_ptr<int> s = make_shared<int>();
    shared_ptr<int> ss = make_shared<int>(20);
    printf("s -> value : %d  ss->value : %d \n", *s, *ss);
    s = ss;
    ss = s;
    printf("s -> value : %d  ss->value : %d \n", *s, *ss);

    unique_ptr<int> uptr = make_unique<int>(20);
    printf("uptr -> value : %d  \n", *uptr);
    auto utmp = std::move(uptr);
    if(!uptr)
        printf("utmp -> value : %d  \n", *utmp);
    else
        printf("uptr -> value : %d, utmp -> value : %d  \n", *uptr, *utmp);

    int a = 20;
    int b = 30;
    auto g = [a, b](){
        return a + b;
    };

    printf("ret = %d \n", g());
    printf("a = %d, b = %d \n", a, b);
    a = 40;
    b = 50;

    auto f = [a, b](int x, int y){
        return a + b + x + y;
    };
    printf("ret = %d \n", f(a, b));
    printf("a = %d, b = %d \n", a, b);

    future<int> fut = std::async([](int a, int b){
        return a + b;
    }, 20, 30);
    printf("fut -> value : [%d] \n", fut.get());


    thread t([](int a, int b){sleep(10); printf("a = [%d], b = [%d] \n", a, b);}, 2000, 3000);
    t.join();
    
    

    return 0;
}*/