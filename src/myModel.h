#pragma once

#include <stdio.h>

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
// GL Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>// Contains all the necessery OpenGL includes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#include <glm/mat4x4.hpp> // MVP Matrix update

// #include <Shader.hpp>



class CMaterial;
class CMesh;
class CModel;



class CTexture
{

public:
    GLuint textureID;
    string name;

    CTexture( const char* fullpath )
    {
        textureID = TextureFromFile( fullpath );
    }


    GLint TextureFromFile(const char* fullpath )
    {
        //Generate texture ID and load texture data 
        // string filename = string(path);
        // filename = directory + '/' + filename;
        GLuint textureID;
        glGenTextures(1, &textureID);
        int width,height;
        unsigned char* image = SOIL_load_image(fullpath, &width, &height, 0, SOIL_LOAD_RGB);
        // Assign texture to ID
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);    

        // Parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
        return textureID;
    }
};




typedef struct STriangle
{
    int posID[3]; //  = {0,0,0};      // position ID
    int texCoID[3]; //  = {0,0,0};    // texture Coordiante ID
    int normalID[3]; //  = {0,0,0};   // normal ID  
} triangle_t;



void set(triangle_t* tri, int attr, int v_no, int value )
{
    switch( attr )
    {
        case 0: tri->posID[v_no] = value; break;
        case 1: tri->texCoID[v_no] = value; break;
        case 2: tri->normalID[v_no] = value; break;     
    }
}




typedef struct SVertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
} vertex_t;




class CMaterial
{
public:

    Shader* shader = NULL;

    CModel* parent;

    string name;

    glm::vec3   diffuse_color;
    glm::vec3   specular_color;
    float       hardness;

    vector<CTexture> textures;

    CMaterial( vector<string> mtl_description, CModel* parent );
    ~CMaterial();
    void activate();
    void deactivate();

};




class CModel
{

public:
    // each mesh uses exactly one material = shader
    vector<CMesh*> meshes;
    map<string, CMaterial*> materials;

    const float* MVP;
    int frames;

    CModel( string obj_path );

    CModel( vector<glm::vec3>*  allpos, 
            vector<glm::vec3>*  allnormals, 
            vector<glm::vec2>*  allTexCoords,
            vector<triangle_t>*  allTrisIDs );

    ~CModel();

    void draw( );
    void parseMTL( string mtl_path );
    void parseOBJ( string obj_path );
    void updateMVP( const float* MVP, int frames );

    const float* getMVP()
    {
        return this->MVP;
    }
        
};





// one mesh one material one shader
class CMesh
{
    // each 3 vertices represent a triangle-face

public:
    CMaterial* material;
    vector<vertex_t> vertices;
    GLuint VAO, VBO;

    // string name;
    string description;

    CMesh(  string              description, 
            vector<glm::vec3>*  allpos, 
            vector<glm::vec3>*  allnormals, 
            vector<glm::vec2>*  allTexCoords,
            vector<triangle_t>* allTrisIDs,
            CMaterial*          material );

    void buffer();
    void draw();
};










CMaterial::CMaterial( vector<string> mtl_description, CModel* parent )
{
    // mtl_description will be part from the .mtl file
    // generate shader paths from this file
    // i guess one vertex shader and a fragment shader per  

    this->parent = parent;


    this->name = mtl_description[0].substr(7);

    
    for(vector<string>::iterator line = mtl_description.begin(); line!= mtl_description.end(); line++ )
    {
        // define later here details about the material based on mtl lines
        // std::cout << *line << std::endl;
        // if ( *line.compare(0,2,"f ")==0 ) 
        // {
        // }
    }

    // std::cout << "\"\"\"" << std::endl;

    const GLchar* vertexPath    = "../gfx/shaders/basicShader.vert";
    const GLchar* fragmentPath  = "../gfx/shaders/basicShader.frag";

    this->shader = new Shader(vertexPath, fragmentPath);

    // std::cout << "ShaderID: " << this->shader->programID << std::endl;
    printf("New Material: '%s' Shader ID: %i\n", this->name.c_str(), this->shader->programID );
    
}

CMaterial::~CMaterial()
{
    delete this->shader;
}


void CMaterial::activate()
{
    // printf("Activating Material '%s': Shader ID %i\n", this->name.c_str(), this->shader->programID);
    // GLfloat mvp[16];

    // std::cout << parent->getMVP() << std::endl;

    // glUniformMatrix4fv(glGetUniformLocation(this->shader->programID, "MVP"), 1, GL_FALSE, mvp);
    
    // std::cout << "MVP Location: " << glGetUniformLocation(this->shader->programID, "MVP") << std::endl;
    // std::cout << "MVP Location: " << parent->MVP << std::endl;
    this->shader->use();
    glUniformMatrix4fv(glGetUniformLocation(this->shader->programID, "MVP"), 1, GL_FALSE, parent->MVP);

    glUniform1i(glGetUniformLocation(this->shader->programID, "frames"), parent->frames);
    // std::cout << "framess Location: " << glGetUniformLocation(this->shader->programID, "frames") << std::endl;

    // printf("/activate\n");

    // load all textures as well
}


void CMaterial::deactivate()
{
    
    // printf("deactivate\n");
    // glUseProgram(0);

    // unbind all textures!
}





CMesh::CMesh(   string              description,
                vector<glm::vec3>*  allpos, 
                vector<glm::vec3>*  allnormals, 
                vector<glm::vec2>*  allTexCoords,
                vector<triangle_t>* allTrisIDs,
                CMaterial*          material )
{
    this->material = material;
    this->description = description;

    printf("\nNew Mesh     : '%s'\n", this->description.c_str() );
    printf("Used Material: '%s'\n", this->material->name.c_str() );

    if(allTexCoords->size()<1)
    {

         allTexCoords->push_back( glm::vec2(0,0) );
         allTexCoords->push_back( glm::vec2(0,0) );
    }

    if(allnormals->size()<1)
    {

         allnormals->push_back( glm::vec3(0,0,0) );
         allnormals->push_back( glm::vec3(0,0,0) );
    }


    // print input values
    // printf("Input Values\n");
    // for(vector<glm::vec3>::iterator ap = allpos->begin(); ap!= allpos->end(); ap++ )
    // {
    //     printf("%i: %f %f %f\n", ap-allpos->begin(), (*ap)[0], (*ap)[1], (*ap)[2] );
    // }

    for (vector<triangle_t>::iterator tri = allTrisIDs->begin(); tri != allTrisIDs->end(); ++tri)
    {   
        for(int i=0; i<3; i++)
        {
            vertex_t vertex;
            vertex.position = (*allpos)[ tri->posID[i]-1 ];
            vertex.normal   = (*allnormals)[ tri->texCoID[i]-1 ];
            vertex.texCoord = (*allTexCoords)[ tri->normalID[i]-1 ];
            // printf("LocID % 2i : %f %f %f\n", tri->posID[i], vertex.position[0], vertex.position[1], vertex.position[2] );    
            vertices.push_back( vertex );   

        } 
        // printf("\n"); 
    }


    printf("Vertices : %i\n", allpos->size() );
    printf("Normals  : %i\n", allnormals->size() );
    printf("TexCoords: %i\n", allTexCoords->size() );
    printf("Triangles: %i\n", allTrisIDs->size() );
    printf("Buffered : %i (%i) | Vertices-Tuples representing each a Tris \n", vertices.size(), vertices.size()/3 );


    this->buffer();
 

    #if 0 // show buffer content, 3-verts repr tri
    for(vector<vertex_t>::iterator vert= vertices.begin(); vert!=vertices.end(); vert++ )
    {
            if( int(vert - vertices.begin())%3==0)
                printf("\n");
            printf("% 3i: %f %f %f\n", vert - vertices.begin(), vert->position[0], vert->position[1], vert->position[2] );
    }
    printf("\n\n");
    #endif
}


void CMesh::buffer()
{
    // VAO serves as a container
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    
    glBindBuffer( GL_ARRAY_BUFFER, this->VBO );             // make this one aktive for loading, changing

    glBufferData( GL_ARRAY_BUFFER,                          // GLenum target,
                  this->vertices.size() * sizeof(vertex_t),  // GLsizeiptr size,
                 &this->vertices[0],                        // const GLvoid * data,
                  GL_STATIC_DRAW );                         // GLenum usage);


    GLint position_ptr  = glGetAttribLocation(this->material->shader->programID, "position");
    GLint texCoords_ptr = glGetAttribLocation(this->material->shader->programID, "uv");
    GLint normal_ptr    = glGetAttribLocation(this->material->shader->programID, "normal");

    // printf("Pos: %i UV: %i Normal: %i\n", position_ptr, texCoords_ptr, normal_ptr);

     // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(position_ptr);   
    glVertexAttribPointer(position_ptr, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)0);
    // Vertex Texture Coords
    glEnableVertexAttribArray(texCoords_ptr);   
    glVertexAttribPointer(texCoords_ptr, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, texCoord));
    // Vertex Normals
    glEnableVertexAttribArray(normal_ptr);   
    glVertexAttribPointer(normal_ptr, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, normal));

    
    glBindVertexArray(0);

    /////// from: https://learnopengl.com/#!Model-Loading/Mesh
    // glGenVertexArrays(1, &this->VAO);
    // glGenBuffers(1, &this->VBO);
    // glGenBuffers(1, &this->EBO);

    // glBindVertexArray(this->VAO);
    // // Load data into vertex buffers
    // glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // // A great thing about structs is that their memory layout is sequential for all its items.
    // // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // // again translates to 3/2 floats which translates to a byte array.
    // glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);


    // // Set the vertex attribute pointers
    // // Vertex Positions
    // glEnableVertexAttribArray(0);   
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // // Vertex Normals
    // glEnableVertexAttribArray(1);   
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // // Vertex Texture Coords
    // glEnableVertexAttribArray(2);   
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    // glBindVertexArray(0);
    ////////////////
}

void CMesh::draw( )
{
    // texture loading is still missing here
    // this->material->shader->use();
    this->material->activate();
    // shader.use();
    glBindVertexArray(this->VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    // glBindVertexArray(this->VBO );
    // ?? glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    // printf("Drawing Arrays...\n");

    // only debugging
    // for(vector<vertex_t>::iterator vert= vertices.begin(); vert!= vertices.end(); vert++)
    // {
    //     // if( int(vert - vertices.begin())%3==0)
    //     // printf("Vertex: %f %f %f\n", vert->position[0], vert->position[1], vert->position[2] );
    // }

    // printf("Drawing %i Vertices. VAO: %i\n", this->vertices.size(), this->VAO );

    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() );

    // fancy disapear of triangles
    // glDrawArrays(GL_TRIANGLES, 0, this->vertices.size()-(this->material->parent->frames*2 % this->vertices.size()) );


    // free Vertex Array
    // glBinkdBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // printf("drawing mesh done\n");
    
    this->material->deactivate();
}








void CModel::draw( )
{
    int i = 0;
    // std::cout << "Meshes in Total: "<< meshes.end()-meshes.begin() << std::endl;
    for(vector<CMesh*>::iterator it_mesh = meshes.begin(); it_mesh != meshes.end(); it_mesh++ )
    {
        i++;
        (*it_mesh)->draw( );
        // std::cout << "Meshes drawn: " << i << std::endl;
    }

    // printf("drawing object done\n");

    // std::cout << "MVP Adress:" << this->MVP << std::endl;
}


void CModel::parseMTL( string mtl_path )
{
    ifstream modellfile( mtl_path.c_str() );

    string          line;
    vector<string>  lines_buffer;
    string          buffered_name;


    printf("  Parsing mtl file...\n");

    if (modellfile.good())
    {
        // CMaterial* current_material = NULL;

        while ( getline(modellfile, line) )
        {
            if(line[0]=='#' or line.size()<4)
            {
                continue;
            }

            if ( line.compare(0,7,"newmtl ")==0 )
            {

                if (lines_buffer.size()>4)
                {
                    // printf("Buffered Name: %s\n", buffered_name.c_str());
                    
                    // std::cout << lines_buffer << std::endl;
                    materials[buffered_name] = new CMaterial(lines_buffer, this );
                    // printf("MATERIAL %s: %x\n", buffered_name.c_str(), materials[buffered_name] );
                    lines_buffer.clear();
                }

                buffered_name = line;
                buffered_name.replace(0,7,"");       // strip 'newmtl ''

            }

            // std::cout <<"LINE '" << line << "'" << std::endl;
            lines_buffer.push_back(line);
        }

        // printf("Buffered Name: %s\n", buffered_name.c_str());
        // create also last material
        materials[buffered_name] = new CMaterial(lines_buffer, this );
        // printf("MATERIAL last %s: %x\n", buffered_name.c_str(), materials[buffered_name] );
    }

}




int string_to_number( string buffer )
{
    int res = atoi( buffer.c_str() );
    // printf("'%s' -> %i\n", buffer.c_str(), res);

    return res;
}




void CModel::parseOBJ( string obj_path )
{
    // make sure mtl is already parsed
    // if no mtl was available create generic material + shader 

    printf("  Parsing Obj..\n");
    
    ifstream modellfile( obj_path.c_str() );
    string line;
    vector<string> lines;

    if (modellfile.good())
    {
        while ( getline(modellfile, line) )
        {
            lines.push_back( line );
        }
    }
    modellfile.close();

    vector<glm::vec3> buffer_positions;
    vector<glm::vec3> buffer_normals;
    vector<glm::vec2> buffer_texCoords;
    vector<triangle_t> allTrisIDs;

    string obj_description;

    CMaterial* current_material = NULL;

    for(vector<string>::iterator line=lines.begin(); line!=lines.end(); line++ )
    {
        // comments and trash
        if((*line)[0]=='#' or (*line)[0]=='s' or line->size()<3)
        {
            continue;
        }


        // new object announced
        if( line->compare(0,2,"o ")==0 ) 
        {   
            if(allTrisIDs.size()>4)
            {
                // printf("Current Material after new object: x%x\n", current_material );

                // printf("Make Mesh: Tris detected: %i \n", allTrisIDs.size() );
                meshes.push_back( new CMesh(    obj_description,
                                                &buffer_positions,
                                                &buffer_normals,
                                                &buffer_texCoords,
                                                &allTrisIDs,
                                                current_material ));
            }

            obj_description = (*line).substr(2);

            buffer_positions.clear();
            buffer_normals.clear();
            buffer_texCoords.clear();
            allTrisIDs.clear();
        }




        if ( line->compare(0,2,"v ")==0 ) 
        // process a vertex position
        {
            float x,y,z;
            sscanf(line->c_str(), "%*s %f %f %f", &x,&y,&z);
            buffer_positions.push_back( glm::vec3(x,y,z) );
            // printf("v %f %f %f\n", x,y,z);
            continue;
        }

        // process a vertex texture coordinate
        if ( line->compare(0,3,"vt ")==0 ) 
        {
            float u, v;
            sscanf(line->c_str(), "%*s %f %f", &u, &v);
            // printf("vt %f %f\n", u,v);
            buffer_texCoords.push_back( glm::vec2(u,v) );   
            continue;
        }
        
        // process a vertex normal
        if ( line->compare(0,3,"vn ")==0 ) 
        {
            float x,y,z;
            sscanf(line->c_str(), "%*s %f %f %f", &x,&y,&z);
            buffer_normals.push_back( glm::vec3(x,y,z) );   
            // printf("vn %f %f %f\n", x,y,z);
            continue;
        } 



        if ( line->compare(0,7,"usemtl ")==0 )     
        {

            // printf("UseMtl: '%s': x%x\n", material_name.c_str(), current_material );
            
            if(allTrisIDs.size()>0)
            {
                // printf("Make Mesh: Tris detected: %i \n", allTrisIDs.size() );
                meshes.push_back( new CMesh(    obj_description,
                                                &buffer_positions,
                                                &buffer_normals,
                                                &buffer_texCoords,
                                                &allTrisIDs,
                                                current_material ));
            }

            string material_name(*line);
            material_name.replace(0,7,"");       // strip 'newmtl ''
            current_material = materials[material_name];

            allTrisIDs.clear();
        }

        // printf("%s\n", (*line).c_str() );

        // process a vertex normal
        if ( line->compare(0,2,"f ")==0 ) 
        {
            int spaces = 0;
            int slashes = 0;

            int v_no = 0;
            int v_attr = 0;

            string buffer = "";
            triangle_t tri = {{0,0,0},{0,0,0},{0,0,0}};
            // printf("New Tris as: %x\n",  &tri);

            // printf("Tri V0 A0: %i\n", tri.posID[0]);

            for(string::iterator c=line->begin()+2; c!=line->end(); c++)
            {
                // printf("%c|", (*c) );
                if(c==line->end()-1)
                {
                    buffer += (*c);
                    // printf("V: %i Attr: %i \n", v_no,v_attr );
                    set(&tri, v_attr, v_no, string_to_number(buffer) );
                    break;
                }

                if((*c)==' ')
                {
                    // printf("V: %i Attr: %i \n", v_no,v_attr );
                    set(&tri, v_attr, v_no, string_to_number(buffer) );
                    v_no++;
                    v_attr = 0;
                    buffer = "";
                }
                else if((*c)=='/')
                {
                    // printf("V: %i Attr: %i \n", v_no,v_attr );
                    set(&tri, v_attr, v_no, string_to_number(buffer) );
                    // tri.set(v_attr, v_no, atoi( buffer.c_str() ) );
                    v_attr++;
                    buffer = "";
                }
                else
                {
                    buffer += (*c);
                }

            }


            // printf("result: %i/%i/%i %i/%i/%i %i/%i/%i\n", tri.posID[0], tri.texCoID[0], tri.normalID[0], tri.posID[1], tri.texCoID[1], tri.normalID[1], tri.posID[2], tri.texCoID[2], tri.normalID[2] );

            // printf("'%s'  %i/%i/%i %i/%i/%i %i/%i/%i\n", (*line).c_str(), tri );

            allTrisIDs.push_back( tri );    
        }   


        // last line 
        if( line==lines.end()-1 and allTrisIDs.size()>4)
        {
            // printf("Current Material after new object: x%x\n", current_material );

            // printf("Make Mesh: Tris detected: %i \n", allTrisIDs.size() );
            meshes.push_back( new CMesh(    obj_description,
                                            &buffer_positions,
                                            &buffer_normals,
                                            &buffer_texCoords,
                                            &allTrisIDs,
                                            current_material ));
        }            
    }
    printf(" obj parsing done!\n");
}





void CModel::updateMVP( const float* MVP, int frames )
{
    this->MVP = MVP;
    this->frames = frames;
    // for(vector<CMesh>)
}

CModel::CModel( string obj_path )
{

    frames = 0;
    printf("\n\tBegin Model Loading..\n\n");
    
    string mtl_path = obj_path;
    mtl_path.replace(mtl_path.end()-3, mtl_path.end(), "mtl" );
    
    parseMTL( mtl_path );
    parseOBJ( obj_path );

    printf("Meshes loaded:     %i\n", meshes.size());
    printf("Materials loaded:  %i\n", materials.size());

    printf("\n\tLoading done!\n\n");

}




CModel::CModel( vector<glm::vec3>*  allpos, 
                vector<glm::vec3>*  allnormals, 
                vector<glm::vec2>*  allTexCoords,
                vector<triangle_t>*  allTrisIDs )
{
    frames = 0;

    printf("Individual Model, single Mesh, basicShader:\n");

    vector<string> cap;
    cap.push_back("usemtl none");
    CMaterial* current_material = new CMaterial(cap, this );

    meshes.push_back( new CMesh(    "None",
                                    allpos,
                                    allnormals,
                                    allTexCoords,
                                    allTrisIDs,
                                    current_material ));

}



CModel::~CModel()
{
    // delete materials
    // delete meshes
}


