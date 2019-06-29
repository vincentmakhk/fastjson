# fastjson

C++ classes for json serialization.

I came across a use case which I need to parse json strings. Its content is of many depths, but most of the nodes I am not interested in.

Instead of calling deserialization and creating objects, I created my own class.

It's basically first scan the string and store the locations of first-level children node.

Only when depth is needed then it would scan the corresponding portion of the string for the locations of the its children node.

## Built With

There is a sample test.cpp included, with a Visual Studio 2015 project file, to illustrate its usage.

## Authors

Vincent Mak

## License

This project is licensed under the GNU General Public License v3.0 License - see the LICENSE.txt file for details

## Acknowledgments

Thanks for the inspiration when I came across those json data.
