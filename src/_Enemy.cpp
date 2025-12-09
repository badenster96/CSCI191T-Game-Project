#include "_enemy.h"

_enemy::_enemy()
{
    //ctor
    iFrames = 0.3f;
    lastTimeHit = 0.0f;


}

_enemy::~_enemy()
{
    //dtor
}
void _enemy::init(const char* filename){
    initModel(filename);
}
int _enemy::ReadMD2Model(const char* filename, struct md2_model_t* mdl)
{
     FILE *fp;

  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read header */
  fread (&mdl->header, 1, sizeof (struct md2_header_t), fp);

  if ((mdl->header.ident != 844121161) ||
      (mdl->header.version != 8))
    {
      /* Error! */
      fprintf (stderr, "Error: bad version or identifier\n");
      fclose (fp);
      return 0;
    }

  /* Memory allocations */
  mdl->skins = (struct md2_skin_t *)malloc (sizeof (struct md2_skin_t) * mdl->header.num_skins);
  mdl->texcoords = (struct md2_texCoord_t *)malloc (sizeof (struct md2_texCoord_t) * mdl->header.num_st);
  mdl->triangles = (struct md2_triangle_t *)malloc (sizeof (struct md2_triangle_t) * mdl->header.num_tris);
  mdl->frames = (struct md2_frame_t *)malloc (sizeof (struct md2_frame_t) * mdl->header.num_frames);
  mdl->glcmds = (int *)malloc (sizeof (int) * mdl->header.num_glcmds);

  /* Read model data */
  fseek (fp, mdl->header.offset_skins, SEEK_SET);
  fread (mdl->skins, sizeof (struct md2_skin_t),mdl->header.num_skins, fp);

  fseek (fp, mdl->header.offset_st, SEEK_SET);
  fread (mdl->texcoords, sizeof (struct md2_texCoord_t),mdl->header.num_st, fp);

  fseek (fp, mdl->header.offset_tris, SEEK_SET);
  fread (mdl->triangles, sizeof (struct md2_triangle_t),mdl->header.num_tris, fp);

  fseek (fp, mdl->header.offset_glcmds, SEEK_SET);
  fread (mdl->glcmds, sizeof (int), mdl->header.num_glcmds, fp);

  /* Read frames */
  fseek (fp, mdl->header.offset_frames, SEEK_SET);
  for (int i = 0; i < mdl->header.num_frames; ++i)
    {
      /* Memory allocation for vertices of this frame */
      mdl->frames[i].verts = (struct md2_vertex_t *)malloc (sizeof (struct md2_vertex_t) * mdl->header.num_vertices);

      /* Read frame data */
      fread (mdl->frames[i].scale, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].translate, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].name, sizeof (char), 16, fp);
      fread (mdl->frames[i].verts, sizeof (struct md2_vertex_t),mdl->header.num_vertices, fp);

      // cout<<mdl->frames[i].name<<endl; // only for Debug
    }
    for(int i =0; i<mdl->header.num_skins; i++){

        std::string path = "models/";
        path += mdl->skins[i].name;
        if(path.size() > 4 && path.substr(path.size()-4) == ".pcx"){
            path = path.substr(0, path.size() - 4) + ".jpg";
        }
        cout << path << endl;
        path = "models/badboyblake/blake.jpg";
        myTex->loadTexture((char*)path.c_str());
        //myTex->loadTexture("models/Tekk/blade.jpg");
        mdl->tex_id = myTex->textID;
    }
     EndFrame = mdl->header.num_frames-1;

  fclose (fp);
  return 1;

}

// Spawns the enemy at a random location around a point
void _enemy::spawn(vec3 center){
    // Enemy stats
    health = 10.0f + rand()%5;
    damage = 1.0f + rand()%10;
    speed = 0.2f + rand()%100 * 0.001f;
    // Spawn logic
    float radius = rand()%50/10.0f + 10;
    angle = rand()%360 / 180.0f * PI;
    // Calculate the random point
    pos.x = center.x + radius * cos(angle);
    pos.y = 0.0f;
    pos.z = center.z + radius * sin(angle);
    // std::cout << radius << " " << angle << " " << pos.x << pos.z << std::endl;
    isSpawned = isAlive = true;
}
void _enemy::draw() {
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(angle+90, 0, 1, 0);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.1,0.1,0.1);
        Actions();
        Draw();
    glPopMatrix();
}

void _enemy::moveTowardPoint(vec3 point) {
    vec3 path;
    path.x = point.x - pos.x;
    path.y = 0;
    path.z = point.z - pos.z;
    this-> angle = atan2f(path.x, path.z) * 180.0f / PI;
    // Normalize the vector, to control speed better
    float length = sqrtf(path.x*path.x + path.z*path.z);
    if(length > 0.01f){
        path.x /= length;
        path.z /= length;
        actionTrigger = RUN;
        pos.x += path.x * speed;
        pos.z += path.z * speed;

    }
}
