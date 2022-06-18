# lsc
my personal c++ utils (Framework)

Set of small and medium-size libraries I write for not using third-party c++ utilities, excepted the (GNU)libstdc++ 

// Coding Style ( May change during R&D ) 
------------
* Pascal Case (Globally)
* 4 Spaces General Indetation ( no tabs )
* "mMultiWordsAttribute":  Class Attribute Members
    * Pointers:
        * Pointers     : mpAttribute
        * ptr-to-const : mpcAttribute
        * const ptr    : mpcAttribute
        * const ptr-to-const : mcpcAttribute
    * Access:
        * Public    : mAttribute
        * Protected : _mAttribute     
        * Private   : _mAttribute
    * Single letters attribute members are not by the above rules.
    * ex:
        * Rect.a.x
        * Point.x, Point.y
        * Size.w, Size.h
        * ...
    * Single letter member function is Uppercase.
     
* No std smart pointers 
* Braces Openning Bloc on new line
* ...

