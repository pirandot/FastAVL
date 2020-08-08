#include <iostream>
#include <bitset>
#include "FastAVL.hpp"

using namespace std;





void lines (int n = 1)
{
    for (int i = 0; i < n; i++) {
        cout << "========================================";
    }
    cout << endl;
}





void testA ()
{
    try {
        AVL_Tree<int, NoVal>*  tree = new AVL_Tree<int, NoVal>;

        cout << "========================================" << endl;
        cout << ">>>   FastAVL – Test – Simple        <<<" << endl;
        cout << "========================================" << endl;
        cout << endl;

        int                     a[] =
        {
            100, 55, 50, 45, 47, 70, 80, 78, 77, 79,
            82, 81, 83, 150, 140, 135, 142, 143, 180, 170,
            165, 160, 175, 173, 200, 190, 195, 500, 1000, 1500,
            82, 81, 83, 150, 140, 135, 142, 143, 180, 170,
            165, 160, 175, 173, 200, 190, 195, 500, 1000, 1500,
            2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
            165, 160, 175, 173, 200, 190, 195, 500, 1000, 1500,
            9999
        };
        int                     b[] =
        {
            45, 47, 50, 55, 70, 77, 78, 79, 80, 81,
            82, 83, 100, 135, 140,
            82, 81, 83, 150, 140, 135, 142, 143, 180, 170,
            165, 160, 175, 173, 200, 190, 195, 500, 1000, 1500
        };
        int                     c[] =
        {
            45, 47, 50, 55, 70, 1111, 5000
        };

        tree->display();

        cout << endl;
        lines(2);
        cout << ">>> Normal inserting …" << endl;
        lines(2);
        cout << endl;

        for(auto x : a){
            try {
                lines();
                cout << "Inserted: " << tree->insert(x)->getKey() << endl;
                lines();
                cout << endl;
                tree->display();
            } catch (const char * s) {
                cout << ">>> Caught: (" << x << ") " << s << endl << endl;
            }
        }

        cout << endl;
        lines(2);
        cout << ">>> Normal removing …" << endl;
        lines(2);
        cout << endl;

        for(auto x : b){
            try {
                lines();
                cout << "Removed: " << x << endl;
                lines();
                cout << endl;
                tree->remove(x);
                tree->display();
            } catch (const char * s) {
                cout << ">>> Caught: (" << x << ") " << s << endl << endl;
            }
        }

        cout << endl;
        lines(2);
        cout << ">>> Safe removing …" << endl;
        lines(2);
        cout << endl;

        for(auto x : c){
            try {
                lines();
                cout << "Safely removed: " << x << endl;
                lines();
                cout << endl;
                tree->safeRemove(x);
                tree->display();
            } catch (const char * s) {
                cout << ">>> Caught: " << s << endl << endl;
            }
        }

        cout << endl;
        lines(2);
        cout << ">>> Safe inserting …" << endl;
        lines(2);
        cout << endl;

        for(auto x : b){
            try {
                lines();
                cout << "Safely inserted: " << x << endl;
                lines();
                cout << endl;
                tree->safeInsert(x);
                tree->display();
            } catch (const char * s) {
                cout << ">>> Caught: " << s << endl << endl;
            }
        }

        cout << endl;
        lines(2);
        cout << ">>> Producing errors …" << endl;
        lines(2);
        cout << endl;

        try {
            lines();
            cout << "Producing error, unsafe inserting: " << 9000 << endl;
            lines();
            tree->insert(9000);
        } catch (const char * s) {
            cout << ">>> Caught: " << s << endl << endl;
        }

        try {
            lines();
            cout << "Producing error, unsafe removing: " << 5555 << endl;
            lines();
            tree->remove(5555);
        } catch (const char * s) {
            cout << ">>> Caught: " << s << endl << endl;
        }
    } catch (const char * s) {
        cout << "Something strange is gonna happen: " << s << endl;
    }
}





class KeyType
{
public:
    bitset<64>              elems;
    unsigned char           start;
    unsigned char           end;

    KeyType()
    {
        elems = {};
        start = 0;
        end   = 0;
    }

    KeyType(bitset<64> el, u_char s, u_char e)
    {
        elems = el;
        start = s;
        end   = e;
    }

    KeyType(const KeyType& k)   // für den AVL_Baum
    {
        elems = k.elems;
        start = k.start;
        end   = k.end;
    }

    KeyType& operator= (const KeyType &) = default;

    bool operator== (const KeyType& k)
    {
        return start == k.start  &&  end == k.end  &&  elems.to_ulong() == k.elems.to_ulong();
    }

    bool operator!= (const KeyType& k)
    {
        return start != k.start  ||  end != k.end  ||  elems.to_ulong() != k.elems.to_ulong();
    }

    bool operator< (const KeyType& k)
    {
        if (start < k.start) {
            return true;
        }
        else if (start == k.start) {
            if (end < k.end) {
                return true;
            }
            else if (end == k.end) {
                return elems.to_ulong() < k.elems.to_ulong();
            }
        }
        return false;
    }

    bool operator> (const KeyType& k)
    {
        if (start > k.start) {
            return true;
        }
        else if (start == k.start) {
            if (end > k.end) {
                return true;
            }
            else if (end == k.end) {
                return elems.to_ulong() > k.elems.to_ulong();
            }
        }
        return false;
    }

    friend std::ostream& operator<< (std::ostream& out, const KeyType& k)
    {
        return out << '[' << (int)k.start << '|' << (int)k.end << '|' << k.elems.to_ullong() << ']';
    }
};



class ValType
{
public:
    unsigned long           sizeA;
    unsigned long           sizeB;

    ValType()
    {
        sizeA = 0;
        sizeB = 0;
    }

    void displayVal ()
    {
        cout << "   " << '<' << sizeA << '|' << sizeB << '>';
    }
};





void testB()
{
    try {
        AVL_Tree<KeyType, ValType>*  tree = new AVL_Tree<KeyType, ValType>;
        AVL_Node<KeyType, ValType>*  node;

        cout << endl;
        cout << endl;
        cout << endl;
        cout << "========================================" << endl;
        cout << ">>>   FastAVL – Test – Complex       <<<" << endl;
        cout << "========================================" << endl;
        cout << endl;

        KeyType                 key;

        bitset<64>              el[] =
        {
            bitset<64>( 1),
            bitset<64>( 3),
            bitset<64>( 7),
            bitset<64>( 7),
            bitset<64>(15),
            bitset<64>(31),
            bitset<64>(31),
            bitset<64>(31),
            bitset<64>(47),
            bitset<64>(47),
            bitset<64>(62),
            bitset<64>(14),
            bitset<64>(48),
            bitset<64>(26),
            bitset<64>(53),
            bitset<64>(63),
            bitset<64>(59),
            bitset<64>(57),
            bitset<64>(56),
            bitset<64>(54),
            bitset<64>( 1),
            bitset<64>( 3),
            bitset<64>( 7),
            bitset<64>( 7),
            bitset<64>(15),
            bitset<64>(31),
            bitset<64>(31),
            bitset<64>(31),
            bitset<64>(47),
            bitset<64>(47)
        };
        u_char                  s[] =
        {
            0, 0, 0, 0, 1, 1, 1, 2, 2, 3,
            0, 0, 0, 0, 1, 1, 1, 2, 2, 3,
            0, 0, 0, 0, 1, 1, 1, 2, 2, 3
        };
        u_char                  e[] =
        {
            5, 5, 2, 1, 3, 2, 4, 4, 3, 2,
            5, 5, 2, 1, 3, 2, 4, 4, 3, 2,
            5, 5, 2, 1, 3, 2, 4, 4, 3, 2
        };

        for (int i = 0; i < 30; i++) {
            if (el[i].to_ullong() < 32) {
                key = KeyType(el[i], s[i], e[i]);
                node = tree->safeInsert(key);
                node->sizeA++;
            }
            else {
                key = KeyType(bitset<64>(63 - el[i].to_ullong()), s[i], e[i]);
                node = tree->safeInsert(key);
                node->sizeB++;
            }
            tree->display();
        }
    } catch (const char * s) {
        cout << "Something strange is gonna happen: " << s << endl;
    }
}





int main()
{
    testA ();
    testB ();
    return 0;
}
