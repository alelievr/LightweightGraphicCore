#ifndef MATERIALTABLE_HPP
# define MATERIALTABLE_HPP
# include <iostream>
# include <string>

class		MaterialTable
{
	private:


	public:
		MaterialTable();
		MaterialTable(const MaterialTable&);
		virtual ~MaterialTable(void);

		MaterialTable &	operator=(MaterialTable const & src);
};

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);

#endif
