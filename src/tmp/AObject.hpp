#pragma once
#include <SdlContext.hh>
// Inclus la bibliotheque de mathematiques
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// La classe abstraite representant un objet avec sa position, sa rotation et son echelle
class AObject
{
public:
  AObject() :
    _position(0, 0, 0), // On initialise la position a 0
    _rotation(0, 0, 0), // pareil pour la rotation
    _scale(1, 1, 1) // l'echelle est mise a 1
  {
  }

  virtual ~AObject()
  {}

  // La fonction initialize charge l'objet ou le construit
  virtual bool initialize()
  {
    return (true);
  }

  // La fonction update sert a gerer le comportement de l'objet
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {}

  // La fonction draw sert a dessiner l'objet
  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock) = 0;




  void translate(glm::vec3 const &v)
  {
    _position += v;
  }

  void rotate(glm::vec3 const& axis, float angle)
  {
    _rotation += axis * angle;
  }

  void scale(glm::vec3 const& scale)
  {
    _scale *= scale;
  }

  glm::mat4 getTransformation()
  {
    glm::mat4 transform(1); // On cree une matrice identite

    // On applique ensuite les rotations selon les axes x, y et z
    transform = glm::rotate(transform, _rotation.x, glm::vec3(1, 0, 0)); 


    transform = glm::rotate(transform, _rotation.y, glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, _rotation.z, glm::vec3(0, 0, 1));
    // On effectue ensuite la translation
    transform = glm::translate(transform, _position);
    // Et pour finir, on fait la mise a l'echelle
    transform = glm::scale(transform, _scale);
    return (transform);
  }

protected:
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;

};

/////////////////////////     CUBE         //////////////////////////



//Nous allons donc faire à présent notre classe Cube héritant de AObject :

class Cube : public AObject
{
private:
  // La texture utilisee pour le cube
  gdl::Texture _texture;
  // La geometrie du cube
  gdl::Geometry _geometry;
  // La vitesse de deplacement du cube
  float _speed;

public:
  Cube() { }
  virtual ~Cube() { }

  virtual bool initialize()
  {
    _speed = 10.0f;

    // On charge la texture qui sera affichee sur chaque face du cube
    if (_texture.load("assets/textures/tmp/text.tga") == false)
      {
	std::cerr << "Cannot load the cube texture" << std::endl;
	return (false);
      }

    // on set la color d'une premiere face
    _geometry.setColor(glm::vec4(1, 0, 0, 1));
    // tout les pushVertex qui suivent seront de cette couleur

    // On y push les vertices d une premiere face
    _geometry.pushVertex(glm::vec3(0.5, -0.5, 0.5));
    _geometry.pushVertex(glm::vec3(0.5, 0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, -0.5, 0.5));

    // Les UVs d'une premiere face
    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    // ETC ETC
    _geometry.setColor(glm::vec4(1, 1, 0, 1)); 



    _geometry.pushVertex(glm::vec3(0.5, -0.5, -0.5));
    _geometry.pushVertex(glm::vec3(0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(-0.5, -0.5, -0.5));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    _geometry.setColor(glm::vec4(0, 1, 1, 1));

    _geometry.pushVertex(glm::vec3(0.5, -0.5, -0.5));
    _geometry.pushVertex(glm::vec3(0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(0.5, 0.5, 0.5));
    _geometry.pushVertex(glm::vec3(0.5, -0.5, 0.5));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    _geometry.setColor(glm::vec4(1, 0, 1, 1));

    _geometry.pushVertex(glm::vec3(-0.5, -0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(-0.5, -0.5, -0.5));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    _geometry.setColor(glm::vec4(0, 1, 0, 1));

    _geometry.pushVertex(glm::vec3(0.5, 0.5, 0.5));
    _geometry.pushVertex(glm::vec3(0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, -0.5));
    _geometry.pushVertex(glm::vec3(-0.5, 0.5, 0.5));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    _geometry.setColor(glm::vec4(0, 0, 1, 1));

    _geometry.pushVertex(glm::vec3(0.5, -0.5, -0.5));
    _geometry.pushVertex(glm::vec3(0.5, -0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, -0.5, 0.5));
    _geometry.pushVertex(glm::vec3(-0.5, -0.5, -0.5));

    _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 0.0f));
    _geometry.pushUv(glm::vec2(1.0f, 1.0f));
    _geometry.pushUv(glm::vec2(0.0f, 1.0f));

    // Tres important, on n'oublie pas de build la geometrie pour envoyer ses informations a la carte graphique
    _geometry.build();
    return (true); 


  }


  // Ici le cube bougera avec les fleches du clavier
  virtual void update(gdl::Clock const &clock, gdl::Input &input)
  {
    // On multiplie par le temps ecoule depuis la derniere image pour que la vitesse ne depende pas de la puissance de l'ordinateur
    if (input.getKey(SDLK_UP))
      translate(glm::vec3(0, 0, -1) * static_cast<float>(clock.getElapsed()) * _speed);
    if (input.getKey(SDLK_DOWN))
      translate(glm::vec3(0, 0, 1) * static_cast<float>(clock.getElapsed()) * _speed);
    if (input.getKey(SDLK_LEFT))
      translate(glm::vec3(-1, 0, 0) * static_cast<float>(clock.getElapsed()) * _speed);
    if (input.getKey(SDLK_RIGHT))
      translate(glm::vec3(1, 0, 0) * static_cast<float>(clock.getElapsed()) * _speed);
  }

  virtual void draw(gdl::AShader &shader, gdl::Clock const &clock)
  {
    (void)clock;
    // On bind la texture pour dire que l'on veux l'utiliser
    _texture.bind();
    // Et on dessine notre cube
    _geometry.draw(shader, getTransformation(), GL_QUADS);
  }

};
