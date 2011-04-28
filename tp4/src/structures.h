/* structers */
typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[3];
	GLfloat specular[3];

	GLfloat color[4];

	GLfloat shininess;
} MATERIAL;

typedef enum {t_cube, t_sphere, t_torus, t_icos, t_octa, t_teapot} object_type;
typedef struct
{
	object_type type;

	GLfloat pos[3];
	GLfloat rotAngle;
	
	MATERIAL mat;
} OBJECT;

typedef struct
{
	GLfloat vertex[4][3];
	GLfloat center[3];	
	GLfloat pos[3];
	GLfloat rotAngle;
	
	MATERIAL mat;
} POLY;
