#include <iostream>
#include <cstring>

char*  RmvRow(const char* str, char* str_rmv_row)
{
    size_t index = 0;
    size_t len = strlen(str);

    for(size_t i = 0; i<len; i++)
    {
        if(!(strchr("AEIOUYaeiouy",str[i])))
        {
            str_rmv_row[index++] = str[i];
        }
    }

    str_rmv_row[index] = '\0';
    std::cout<<str_rmv_row<<'\n';
    return str_rmv_row;
}

char* RepSym(const char* str, char* rep_sym)
{
    
    size_t count_symb = 0, index = 0;
    for(size_t i = 0; i<strlen(str); i++)
    {
        count_symb = 0;

        for(size_t v = 0; v<=strlen(str); v++)
        {
            if(str[i] == str[v])
            {
                count_symb++;
            }
        }

        if(count_symb>1)
        {
            if(!strchr(rep_sym,str[i]))
            {
              rep_sym[index++] = str[i];
            }
        }
    }
    rep_sym[index] = '\0';
    std::cout<<rep_sym<<"\n";
    return rep_sym;
}

int main()
{
    size_t start_size = 8, len = 0;
    char* str = new char[start_size];

    char symb = 0;

    while(std::cin.get(symb) && symb != '\n')
    {
        if(len+1 >= start_size)
        {
            start_size *= 2; 
            char* str_new = new char[start_size];
            std::memcpy(str_new,str,len);
            delete[] str;
            str = str_new;
        }
        str[len] = symb;
        len++;
    }

    char str_rmv_row[strlen(str)];
    char rep_sym[strlen(str)];
    RmvRow(str, str_rmv_row);
    RepSym(str,rep_sym);
    delete[] str;
    return 0;
}


