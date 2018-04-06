#include "ProgramChannel.hpp"


ProgramChannel::ProgramChannel(void)
{
	std::cout << "Default constructor of ProgramChannel called" << std::endl;
	this->_programId = 0;
	this->_type = ;
	this->_program = ;
}

ProgramChannel::ProgramChannel(ProgramChannel const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

ProgramChannel::~ProgramChannel(void)
{
	std::cout << "Destructor of ProgramChannel called" << std::endl;
}

int		ProgramChannel::Bind(void)
{
	
}

ChannelType		ProgramChannel::GetType(void)
{
	
}


ProgramChannel &	ProgramChannel::operator=(ProgramChannel const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_programId = src.GetProgramId();
		this->_type = src.GetType();
		this->_program = src.GetProgram();
	}
	return (*this);
}

int		ProgramChannel::GetProgramId(void) const { return (this->_programId); }
void		ProgramChannel::SetProgramId(int tmp) { this->_programId = tmp; }

ChannelType		ProgramChannel::GetType(void) const { return (this->_type); }
void		ProgramChannel::SetType(ChannelType tmp) { this->_type = tmp; }

ShaderProgram		ProgramChannel::GetProgram(void) const { return (this->_program); }
void		ProgramChannel::SetProgram(ShaderProgram tmp) { this->_program = tmp; }

std::ostream &	operator<<(std::ostream & o, ProgramChannel const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
