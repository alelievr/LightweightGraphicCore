#ifndef IMAGECHANNEL_HPP
# define IMAGECHANNEL_HPP
# include <iostream>
# include <string>

class		ImageChannel : public IChannel
{
	private:
		int				_imageId;
		ChannelType		_type;
		std::string		_imagePath;


	public:
		ImageChannel(void);
		ImageChannel(const ImageChannel&);
		virtual ~ImageChannel(void);

		ImageChannel &	operator=(ImageChannel const & src);

		int		Bind(void);

		ChannelType	GetType(void);

		int	GetImageId(void) const;
		void	SetImageId(int tmp);
		
		ChannelType	GetType(void) const;
		void	SetType(ChannelType tmp);
		
		std::string	GetImagePath(void) const;
		void	SetImagePath(std::string tmp);
};

std::ostream &	operator<<(std::ostream & o, ImageChannel const & r);

#endif
