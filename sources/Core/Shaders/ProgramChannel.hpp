#pragma once

#include <iostream>
#include <string>

#include "IChannel.hpp"
#include "ShaderProgram.hpp"
#include "ProgramChannel.hpp"

namespace LWGC
{
	class		ProgramChannel : public IChannel
	{
		private:
			int	_programId;
			ChannelType	_type;
			ShaderProgram	_program;


		public:
			ProgramChannel(void);
			ProgramChannel(const ProgramChannel&);
			virtual ~ProgramChannel(void);

			ProgramChannel &	operator=(ProgramChannel const & src);

			int	Bind(void);

			ChannelType	GetType(void);

			int	GetProgramId(void) const;
			void	SetProgramId(int tmp);
			
			ChannelType	GetType(void) const;
			void	SetType(ChannelType tmp);
			
			ShaderProgram	GetProgram(void) const;
			void	SetProgram(ShaderProgram tmp);
	};

	std::ostream &	operator<<(std::ostream & o, ProgramChannel const & r);
}
