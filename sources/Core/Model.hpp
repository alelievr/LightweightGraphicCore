#ifndef MODEL_HPP
# define MODEL_HPP
# include <iostream>
# include <string>

class		Model
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

		Mesh	getMesh(void) const;
		void	setMesh(Mesh tmp);
		
		std::vector< Material >	getMaterials(void) const;
		void	setMaterials(std::vector< Material > tmp);
};

std::ostream &	operator<<(std::ostream & o, Model const & r);

#endif
