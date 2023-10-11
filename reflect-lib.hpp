#include <iostream>
#include <sstream>
#include "json.hpp"

#define MAPPER(x) template <class R>  void reflect(R &r) { r x; }

using json_t = nlohmann::json;

class JSONWriter
{
    std::ostream &output;
    bool needsComma;

public:
    JSONWriter(std::ostream& out) : output(out)
    {
    }

    template <class T>
    auto write(T &obj) -> decltype(obj.reflect(*this), void())
    {
        output << "{";
        needsComma = false;
        obj.reflect(*this);
        output << "}";
    }

    // template <class T, class M>
    // auto write(T &obj, M* mapper) -> decltype(mapper->reflect(*this), void())
    // {
    //     output << "{";
    //     needsComma = false;
    //     mapper->reflect(*this, mapper);
    //     output << "}";
    // }

    template <class T>
    auto write(T *obj, size_t size) -> decltype(obj->reflect(*this), void())
    {
		int i = 0;

		output << "[";

		for (i = 0; i < size - 1; ++i) {
			output << "{";
			needsComma = false;
			obj[i].reflect(*this);
			output << "},";
		}

		output << "{";
		needsComma = false;
		obj[i].reflect(*this);
		output << "}";
		output << "]";
    }


    void write(int value)
    {
        output << value;
    }

    void write(int *value, size_t size)
    {
	int i = 0;

        output << "[";
        for( i = 0 ; i < size - 1; ++i) {
            output << value[i] << ", ";
        }
        output << value[i] << " ]";
    }

    void write(char *value, size_t _size)
    {
	    int i = 0;
        size_t size = strlen(value) > _size ? _size : strlen(value);

        output << "\"";
        for( i = 0 ; i < size - 1; ++i) {
            output << value[i];
        }
        output << value[i] << "\"";
    }

    void write(std::string &value)
    {
        output << '"' << value << '"';
    }

    template <class T>
    JSONWriter &operator()(const char *name, T &field)
    {
        if (needsComma)
        {
            output << ",";
        }
        needsComma = true;
        output << "\"" << name << "\"" << ":";
        write(field);
        return *this;
    }

    template <class T>
    JSONWriter &operator()(const char *name, T &field, size_t size)
    {
        if (needsComma)
        {
            output << ",";
        }
        needsComma = true;
        output << "\"" << name << "\"" << ":";
        write(field, size);
        return *this;
    }

     std::string getString() {
         output.flush();
         //output.str();
	return "";
     }
};


class JSONReader
{
	json_t &json;
	std::vector<json_t> jsonStack;


public:
    JSONReader(json_t &json) : json(json)
    {
    }

    template <class T>
    JSONReader &operator()(const char *name, T &field)
    {
        read(name, field);
        return *this;
    }

    template <class T>
    JSONReader &operator()(const char *name, T &field, size_t size)
    {
        read(name, field, size);
        return *this;
    }

    template <class T>
    auto read(T &obj) -> decltype(obj.reflect(*this), void())
    {
        obj.reflect(*this);
    }

    template <class T>
    auto read(const char *name, T &obj) -> decltype(obj.reflect(*this), void())
    {
		//std::cout << "auto read called (" << name << ", " << &obj << ")" << std::endl;
   		jsonStack.push_back(json);

		json = json.at(name);

        obj.reflect(*this);
		
		json = jsonStack.back();
		jsonStack.pop_back();
    }

    template <class T>
    auto read(T &obj, size_t size) -> decltype(obj.reflect(*this), void())
    {

		for (int i = 0; i < size ; ++i) {
			obj[i].reflect(*this);
		}
    }

    template <class T>
    auto read(const char *name, T *obj, size_t size) -> decltype(obj->reflect(*this), void())
    {
		//std::cout << "auto read called (" << name << ", " << size << ")" << std::endl;

   		jsonStack.push_back(json);

		//std::cout << "at --> " << &json << std::endl;
		json_t json_array = json.at(name);

		for (int i = 0; i < size ; ++i) {
			json = json_array[i];
			obj[i].reflect(*this);
		}
		
		json = jsonStack.back();
		jsonStack.pop_back();

    }

    void read(const char *name, int &value)
    {
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;

		value = json.at(name);

    }

    void read(const char *name, int *value, size_t size)
    {
		int i = 0;
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;
		
		auto data = json.at(name);


		for( auto it = data.begin(); it != data.end() && i < size ; ++it, ++i) {
			value[i] = it.value();
		}
    }


    void read(const char *name, unsigned int &value)
    {
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;

		value = json.at(name);

    }

    void read(const char *name, unsigned int *value, size_t size)
    {
		int i = 0;
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;
		
		auto data = json.at(name);


		for( auto it = data.begin(); it != data.end() && i < size ; ++it, ++i) {
			value[i] = it.value();
		}
    }


    void read(const char *name, double &value)
    {
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;

		value = json.at(name);

    }

    void read(const char *name, double *value, size_t size)
    {
		int i = 0;
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;
		
		auto data = json.at(name);


		for( auto it = data.begin(); it != data.end() && i < size ; ++it, ++i) {
			value[i] = it.value();
		}
    }


    void read(const char *name, unsigned long &value)
    {
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;

		value = json.at(name);

    }

    void read(const char *name, unsigned long *value, size_t size)
    {
		int i = 0;
    	//std::cout << name << std::endl;

    	//std::cout << name << "   :   " <<  json.at(name) << std::endl;
		
		auto data = json.at(name);


		for( auto it = data.begin(); it != data.end() && i < size ; ++it, ++i) {
			value[i] = it.value();
		}
    }



    void read(const char *name, char *value, size_t _size)
    {
    	std::cout << name << std::endl;
    	std::cout << name << "   :   " <<  json.at(name) << std::endl;


		std::string &ref = json.at(name).get_ref<std::string&>();


        size_t size = strlen(ref.c_str()) > _size ? _size : strlen(ref.c_str());

    	std::cout << "_size = " << _size << " , size = " << size << std::endl;

		memset(value, 0x00, size);
		strncpy(value, ref.c_str(), size);
    	std::cout << "value = " << value << std::endl;
    }

    void read(std::string &value)
    {
 //       output << '"' << value << '"';
    }
};


template <class T> const char *
convert_to_json(T& c_struct) {
    std::ostringstream out;

    JSONWriter jsonWriter(out);
    jsonWriter.write(c_struct);

    return strdup(out.str().c_str());
}

template <class T> const char *
convert_to_json(T* c_struct) {
    std::ostringstream out;

    JSONWriter jsonWriter(out);
    jsonWriter.write(*c_struct);
    //std::cout << "[== " << out.str() << " ==] " << std::endl;
    return strdup(out.str().c_str());
}

template <class T> void
convert_from_json(const char *json_string, T &c_struct) {
    json_t json = json_t::parse(json_string);
    JSONReader jsonReader(json);
    jsonReader.read(c_struct);
}


