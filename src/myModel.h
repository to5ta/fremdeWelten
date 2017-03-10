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

// #include <Shader.hpp>


// #include <Material.h>


// struct Vertex
// {
//     glm::vec3 position;
//     glm::vec2 texCoord;
//     glm::vec3 normal;
// };


// struct Weight
// {
//     int     jointID;
//     float   weight;
// };




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





// class CShader
// {
//     CShader();

// };




class CMaterial
{
public:
    Shader* shader = NULL;
    CMaterial()
    {

    }


};




struct triangle
{
    uint posID[3];      // position ID
    uint texCoID[3];    // texture Coordiante ID
    uint normalID[3];   // normal ID
};


struct SVertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};



// one mesh one material one shader
class CMesh
{

private:

    // each 3 vertices represent a triangle-face
    vector<SVertex> vertices;

    CMaterial* material;

public:
    GLuint VBO;
    // same order + same length!
    // vector<glm::vec3> positions;
    // vector<glm::vec3> normals;
    // vector<glm::vec2> texCoords;
    // vector<uint> matIDs;
    // vector<CMaterial> materials;

    CMesh(  vector<glm::vec3> allpos, 
            vector<glm::vec3> allnormals, 
            vector<glm::vec2> allTexCoords,
            vector<triangle> allTrisIDs )
    {
        for (vector<triangle>::iterator tri = allTrisIDs.begin() ; tri != allTrisIDs.end(); ++tri)
        {   
            for(int i=0; i<3; i++)
            {
                SVertex vertex;
                vertex.position = allpos[ tri->posID[i] ];
                vertex.normal   = allnormals[ tri->texCoID[i] ];
                vertex.texCoord = allTexCoords[ tri->normalID[i] ];
                vertices.push_back( vertex );        
            }  
        }

        this->buffer();
    }


    void buffer()
    {
        glGenBuffers(1, &VBO);

        glBindBuffer( GL_ARRAY_BUFFER, VBO );                       // make this one aktive for loading, changing, rendering

        glBufferData( GL_ARRAY_BUFFER,                          // GLenum target,
                      this->vertices.size() * sizeof(SVertex),  // GLsizeiptr size,
                     &this->vertices[0],                        // const GLvoid * data,
                      GL_STATIC_DRAW );                         // GLenum usage);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);   
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);   
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);   
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

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



    void draw()
    {
        // texture loading is still missing here 

        this->material->shader->use();
        // shader.use();
        glBindVertexArray(this->VBO);
        // ?? glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() );
        glBindVertexArray(0);
        glUseProgram(0);

    }
};






class CModell
{

public:
    
    vector<Mesh> meshes;


    CModell( string path )
    {
        vector<glm::vec3> positions;
        vector<glm::vec3> normals;
        vector<glm::vec2> texCoords;
        ifstream modellfile( path.c_str() );

        string line;

        if (modellfile.good())
        {
            while ( getline(modellfile, line) )
            {
                if(line[0]=='#')
                {
                    continue;
                }

                // process a vertex position
                if ( line.compare(0,2,"v ")==0 ) 
                {
                    float x,y,z;
                    sscanf(line.c_str(), "%*s %f %f %f", &x,&y,&z);
                    positions.push_back( glm::vec3(x,y,z) );
                    printf("v %f %f %f\n", x,y,z);
                    continue;
                }

                // process a vertex texture coordinate
                if ( line.compare(0,3,"vt ")==0 ) 
                {
                    float u, v;
                    sscanf(line.c_str(), "%*s %f %f", &u, &v);
                    printf("vt %f %f\n", u,v);
                    texCoords.push_back( glm::vec2(u,v) );   
                    continue;
                }
                
                // process a vertex normal
                if ( line.compare(0,3,"vn ")==0 ) 
                {
                    float x,y,z;
                    sscanf(line.c_str(), "%*s %f %f %f", &x,&y,&z);
                    normals.push_back( glm::vec3(x,y,z) );   
                    printf("vn %f %f %f\n", x,y,z);
                    continue;
                } 


                // // process a face
                // if ( line.compare(0,2,"f ")==0 ) 
                // {
                //     int v_id, vt_id, vn_id;

                //     sscanf(line.c_str(), "%*s %f/%f %f", &x,&y,&z);
                //     normals.push_back( glm::vec3(x,y,z) );   
                //     printf("vn %f %f %f\n", x,y,z);
                //     continue;
                // }                         
            }


            modellfile.close();
        }
        

        else cout << "Unable to open file"; 

    }

    // ~Modell();


private:

};