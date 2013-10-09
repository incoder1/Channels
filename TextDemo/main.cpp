#include <iostream>

#include <boost/make_shared.hpp>

#include <console.hpp>
#include <iconv_conv.hpp>
#include <readwrite.hpp>


int main()
{
	typedef io::Writer<std::wstring> writer;
	::SetConsoleCP(CP_ACP);
	::SetConsoleOutputCP(CP_ACP);
	try {
		const io::IconvCharsetFactory* chFactory = io::IconvCharsetFactory::instance();
		const io::Charset* uft16 = chFactory->forName("UTF-16LE");
		const io::Charset* cp1251 = chFactory->forName("CP1251");
		io::File file("test.txt");
		file.create();
		writer out(file.openForWrite(), io::PConverter(new io::IconvConverter(uft16, cp1251)));
		out.write(L"Hello World. Привет мир");
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
