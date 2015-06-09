
//Il est maintenant l’heure de tester tout ca.
#include <Game.hh>
#include <Clock.hh>
#include <Input.hh>
#include <SdlContext.hh>
#include <Geometry.hh>
#include <Texture.hh>
#include <BasicShader.hh>
#include <Model.hh>

#include <OpenGL.hh>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <cstdlib>

#include "GameEngine.hpp"

int main()
{
 // On cree son engine
 GameEngine engine;

 if (engine.initialize() == false)
 return (EXIT_FAILURE);
 while (engine.update() == true)
 engine.draw();
 return EXIT_SUCCESS;
} 
