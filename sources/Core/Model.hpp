#ifndef MODEL_HPP
# define MODEL_HPP
# include <iostream>
# include <string>
# include <Object.hpp>

class		Model : public Object
{
	private:
		Mesh	_mesh;
		std::vector< Material >	_materials;


	public:
		Model();
		Model(const Model&);
		virtual ~Model(void);

		Model &	operator=(Model const & src);

		SortingLayer	GetSortingLayer(void);

		Bounds	GetBounds(void);

		void	Render(void);

		Mesh	GetMesh(void) const;
		void	SetMesh(Mesh tmp);
		
		std::vector< Material >	GetMaterials(void) const;
		void	SetMaterials(std::vector< Material > tmp);
};

std::ostream &	operator<<(std::ostream & o, Model const & r);

#endif
