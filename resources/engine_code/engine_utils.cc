#include "engine.h"
// This contains the lower level code

//TinyOBJLoader - This has to be included in a .cc file, so it's here for right now
#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_DOUBLE
#include "../TinyOBJLoader/tiny_obj_loader.h"


void engine::create_window()
{
    if(SDL_Init( SDL_INIT_EVERYTHING ) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

    // this is how you query the screen resolution
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    // pulling these out because I'm going to try to span the whole screen with
    // the window, in a way that's flexible on different resolution screens
    int total_screen_width = dm.w;
    int total_screen_height = dm.h;

    cout << "creating window...";

    // window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS );
    window = SDL_CreateWindow( "OpenGL Window", 0, 0, total_screen_width, total_screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE );
    SDL_ShowWindow(window);

    cout << "done." << endl;


    cout << "setting up OpenGL context...";
    // OpenGL 4.3 + GLSL version 430
    const char* glsl_version = "#version 430";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    GLcontext = SDL_GL_CreateContext( window );

    SDL_GL_MakeCurrent(window, GLcontext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    // SDL_GL_SetSwapInterval(0); // explicitly disable vsync

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);

    glPointSize(3.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, GLcontext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    clear_color = ImVec4(75.0f/255.0f, 75.0f/255.0f, 75.0f/255.0f, 0.5f); // initial value for clear color

    // really excited by the fact imgui has an hsv picker to set this
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( window );

    cout << "done." << endl;

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.64f, 0.37f, 0.37f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.49f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.17f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.18f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.35f, 0.00f, 0.03f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f, 0.04f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.87f, 0.23f, 0.09f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.81f, 0.38f, 0.09f, 0.08f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void engine::gl_setup()
{
    // some info on your current platform
    const GLubyte *renderer = glGetString( GL_RENDERER ); // get renderer string
    const GLubyte *version = glGetString( GL_VERSION );  // version as a string
    printf( "Renderer: %s\n", renderer );
    printf( "OpenGL version supported %s\n\n\n", version );



    // create the shader for the triangles to cover the screen
    display_shader = Shader("resources/engine_code/shaders/blit.vs.glsl", "resources/engine_code/shaders/blit.fs.glsl").Program;

    // set up the points for the display
    //  A---------------B
    //  |          .    |
    //  |       .       |
    //  |    .          |
    //  |               |
    //  C---------------D

    // diagonal runs from C to B
    //  A is -1, 1
    //  B is  1, 1
    //  C is -1,-1
    //  D is  1,-1
    std::vector<glm::vec3> points;

    points.clear();
    points.push_back(glm::vec3(-1, 1, 0.5));  //A
    points.push_back(glm::vec3(-1,-1, 0.5));  //C
    points.push_back(glm::vec3( 1, 1, 0.5));  //B

    points.push_back(glm::vec3( 1, 1, 0.5));  //B
    points.push_back(glm::vec3(-1,-1, 0.5));  //C
    points.push_back(glm::vec3( 1,-1, 0.5));  //D

    // vao, vbo
    cout << "  setting up vao, vbo for display geometry...........";
    glGenVertexArrays( 1, &display_vao );
    glBindVertexArray( display_vao );

    glGenBuffers( 1, &display_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, display_vbo );
    cout << "done." << endl;

    // buffer the data
    cout << "  buffering vertex data..............................";
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), &points[0]);
    cout << "done." << endl;

    // set up attributes
    cout << "  setting up attributes in display shader............";
    GLuint points_attrib = glGetAttribLocation(display_shader, "vPosition");
    glEnableVertexAttribArray(points_attrib);
    glVertexAttribPointer(points_attrib, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (static_cast<const char*>(0) + (0)));
    cout << "done." << endl;


    int num = 24;
    
    std::string filename = std::string("input_samples/" + std::to_string(num) + ".png");
    unsigned error;

    if((error = m.in.load(filename)))
        cout << endl << "decode error: " << lodepng_error_text(error);
    else
        cout << endl << "Image (" << filename << ") loaded. Width: " << m.in.width << "  Height: " << m.in.height << endl;

    
    m.acquire_colors();
    m.tile_parse();
    m.tile_sort();
    // m.dump_tiles(std::string("tile" + std::to_string(num) + ".png"));
    m.dump_tiles(std::string("tiles.png"));

    // m.construct_output();
    // m.collapse();
    
    // create the image textures
    glGenTextures(1, &display_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, display_texture);

    // texture parameters
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // buffer the image data to the GPU
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, display_texture);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8UI, m.in.width, m.in.height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &m.in.data[0]);
    glBindImageTexture(0, display_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8UI);


}


unsigned image::load(std::string filename)
{
    return lodepng::decode(data, width, height, filename.c_str());
}

int image::at(int x, int y)
{
    // if the pixel is off the image
    if(x >= (int)width || x < 0 || y >= (int)height || y < 0)
        return 0; // represents 'off'

    // get the pixel from the image
    int index = (y * width + x) * 4;
    glm::ivec3 select = glm::ivec3(data[index], data[index+1], data[index+2]);

    // check against palette, return index of matching color
    for(unsigned i = 1; i < colors.size(); i++)
        if(select == colors[i])
        {
            return i;
        }

    // shouldn't see this happen, but have to include for the compiler
    return 0;
}

void image::count_colors()
{
    // index 0 is reserved for 'off'
    colors.resize(1, glm::ivec3(-1));

    // prime the pump with the first color (because of how the inner for-loop below operates)
    colors.push_back(glm::ivec3(data[0], data[1], data[2]));

    // get the rest of the colors in the data array
    glm::ivec3 current;
    for(unsigned i = 4; i < data.size(); i+=4)
    {
        current = glm::ivec3(data[i], data[i+1], data[i+2]); // grab RGB, ignore alpha
        bool seen = false;
        
        for(unsigned j = 1; j < colors.size(); j++)
        {
            if(current == colors[j])
            {
                seen = true; // we have already seen this color
            }
        }
        
        if(!seen)
        {
            colors.push_back(current); // this is a new color
            // cout << endl << "added color " << current.x << " " << current.y << " " << current.z << endl;
        }
    }
}

void model::acquire_colors()
{
    in.count_colors();
    if(in.colors.size() > (1+MAX_COLORS)) // 0 represents 'off', followed by the list of indexed colors
    {
        cout << endl << endl << "Too many colors in input image. Aborting." << endl << endl;
        abort();
    }
    else
    {
        cout << endl << endl << "Color count found " << in.colors.size()-1 << " colors." << endl << endl;
    }
}


void model::tile_parse()
{
    // get every 3x3 neighborhood from the image - uses image.at() which internally does bounds checking
    //  this will eventually also do rotations and mirrored rotations

    tile temp;
    temp.count = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    
    for(int x = 0; x < (int)in.width; x++)
    {
        // clear percentage reporting
        percent_done.str(std::string());
        // report new percentage
        percent_done << "Image " << (int)(((float)x/(float)in.width)*100) << " percent parsed. " << std::flush;

        for(int y = 0; y < (int)in.height; y++)
        {
            temp.data[0] = in.at(x-1, y-1);
            temp.data[1] = in.at(  x, y-1);
            temp.data[2] = in.at(x+1, y-1);
            
            temp.data[3] = in.at(x-1,   y);
            temp.data[4] = in.at(  x,   y);
            temp.data[5] = in.at(x+1,   y);

            temp.data[6] = in.at(x-1, y+1);
            temp.data[7] = in.at(  x, y+1);
            temp.data[8] = in.at(x+1, y+1);

            // add_tile(temp); // original
            // add_tile(temp.rotate()); // rotated 90 degrees ccw
            // add_tile(temp.rotate().rotate()); // rotated 180 degrees ccw
            // add_tile(temp.rotate().rotate().rotate()); // rotated 270 degrees ccw
            // add_tile(temp.mirror()); // mirror 
            // add_tile(temp.mirror().rotate()); // mirrored, rotated 90 degrees ccw
            // add_tile(temp.mirror().rotate().rotate()); // mirrored, rotated 180 degrees ccw
            // add_tile(temp.mirror().rotate().rotate().rotate()); // mirrored, rotated 270 degrees ccw
            switch(SYMMETRY)
            {
                case 8: add_tile(temp.mirror().rotate().rotate().rotate()); // mirror, three ccw rotations
                case 7: add_tile(temp.mirror().rotate().rotate());         // mirror, two ccw rotations
                case 6: add_tile(temp.mirror().rotate());                 // mirror, one ccw rotation
                case 5: add_tile(temp.mirror());                         // mirrored original
                case 4: add_tile(temp.rotate().rotate().rotate());      // three ccw rotations
                case 3: add_tile(temp.rotate().rotate());              // two ccw rotations
                case 2: add_tile(temp.rotate());                      // oen ccw rotation
                case 1: add_tile(temp);                              // original
                    break;
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    
    cout << "\rParsing complete. Found " << tiles.size() << " distinct tiles, of a possible " << 8*in.width*in.height << " ("
         << ((float)tiles.size() / (float)(8*in.width*in.height))*100.0 << "%) in "
         << std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count() << " seconds." << endl;
}

void model::add_tile(tile new_tile)
{
    // go through the list of tiles
    for(auto& t : tiles)
    {
        // if you find the tile, increment its count, then return
        if(new_tile == t)
        {
            t.count++;
            return;
        }
    }
    
    // else, if you drop out of the loop without finding a match, push onto the back of the list
    tiles.push_back(new_tile);
    
    // report new tile count
    std::cout << "\r" << percent_done.str() << "Distinct tile count: " << tiles.size() << std::flush;
}

void model::tile_sort()
{
    //uses default, < operator, to sort the list of tiles by the number of occurrences
    std::sort(tiles.begin(), tiles.end());
    
    // I want the largest count first, not last
    std::reverse(tiles.begin(), tiles.end());
}

void model::dump_tiles(std::string filename)
{
    // creates an output image of all parsed tiles, in the order that they appear in the tiles vector
    int n = 150; // how many tiles per row e.g. <tile 0> <tile 1> ... <tile n-1> then a new row starting w <tile n>

    int height, width;
    width = (4 * n) + 1;
    height = 0;
   
    std::vector<unsigned char> image_data;
    image_data.resize(0);
    
    for(int offset = 0; offset < (int)tiles.size(); offset += n) // rows of tiles
    {
        // row of transparent pixels above these tiles (first row in image, then divider between each)
        for(int i = 0; i < (4*width); i++) // four pixels per tile including transparent leader, four components, plus the final transparent pixel 
            image_data.push_back(0);
            
        // first slice of these tiles
        for(int i = 0; i < n; i++) //four pixels per tile {transparent, data[0], data[1], data[2]}
        {
            // TRANSPARENT
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 

            if(offset+i < (int)tiles.size())
            {
                // DATA[0]
                image_data.push_back(in.colors[tiles[offset+i].data[0]].x);
                image_data.push_back(in.colors[tiles[offset+i].data[0]].y);
                image_data.push_back(in.colors[tiles[offset+i].data[0]].z);
                image_data.push_back(255);

                // DATA[1]
                image_data.push_back(in.colors[tiles[offset+i].data[1]].x);
                image_data.push_back(in.colors[tiles[offset+i].data[1]].y);
                image_data.push_back(in.colors[tiles[offset+i].data[1]].z);
                image_data.push_back(255);

                // DATA[2]
                image_data.push_back(in.colors[tiles[offset+i].data[2]].x);
                image_data.push_back(in.colors[tiles[offset+i].data[2]].y);
                image_data.push_back(in.colors[tiles[offset+i].data[2]].z);
                image_data.push_back(255);
            }
            else
            {
                for(int j = 0; j < 12; j++)
                    image_data.push_back(0);
            }
        }
            
        // terminate with transparent pixel
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        

        // second slice of these tiles
        for(int i = 0; i < n; i++) //four pixels per tile {transparent, data[3], data[4], data[5]}
        {
            // TRANSPARENT
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 

            if(offset+i < (int)tiles.size())
            {
            // DATA[3]
            image_data.push_back(in.colors[tiles[offset+i].data[3]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[3]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[3]].z);
            image_data.push_back(255);

            // DATA[4]
            image_data.push_back(in.colors[tiles[offset+i].data[4]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[4]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[4]].z);
            image_data.push_back(255);

            // DATA[5]
            image_data.push_back(in.colors[tiles[offset+i].data[5]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[5]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[5]].z);
            image_data.push_back(255);
            }
            else
            {
                for(int j = 0; j < 12; j++)
                    image_data.push_back(0);
            }
        }
            
        // terminate with transparent pixel
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        
        // third slice of these tiles
        for(int i = 0; i < n; i++) //four pixels per tile {transparent, data[6], data[7], data[8]}
        {
            // TRANSPARENT
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 
            image_data.push_back(0); 

            if(offset+i < (int)tiles.size())
            {
            // DATA[6]
            image_data.push_back(in.colors[tiles[offset+i].data[6]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[6]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[6]].z);
            image_data.push_back(255);

            // DATA[7]
            image_data.push_back(in.colors[tiles[offset+i].data[7]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[7]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[7]].z);
            image_data.push_back(255);

            // DATA[8]
            image_data.push_back(in.colors[tiles[offset+i].data[8]].x);
            image_data.push_back(in.colors[tiles[offset+i].data[8]].y);
            image_data.push_back(in.colors[tiles[offset+i].data[8]].z);
            image_data.push_back(255);
            }
            else
            {
                for(int j = 0; j < 12; j++)
                    image_data.push_back(0);
            }
        }
            
        // terminate with transparent pixel
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        image_data.push_back(0); 
        
        height += 4;
    }


    //add final row of transparent pixels
    height++;
    for(int i = 0; i < (4*width); i++) // four pixels per tile including transparent leader, four components, plus the final transparent pixel 
        image_data.push_back(0);

    // dump the image data
    // std::string filename = std::string("tiles.png");

    unsigned error = lodepng::encode(filename.c_str(), image_data, width, height);
    if(error) cout << endl << "decode error: " << lodepng_error_text(error);

}

// void model::construct_output()
// {
//     out.resize(width); // initialize the 2d array of output tiles
//     for(auto& col : out)
//         col.resize(height, output_tile(tiles.size()));
// }

void model::collapse()
{
    
}


output_tile::output_tile(int tile_count)
{
    state = UNKNOWN;

    for(int i = 0; i < tile_count; i++)
        possible_tiles.push_back(i);
}


void engine::draw_everything()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io; // void cast prevents unused variable warning
    //get the screen dimensions and pass in as uniforms


    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   // from hsv picker
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // clear the background

    // draw the stuff on the GPU

    // texture display
    glUseProgram(display_shader);
    glBindVertexArray( display_vao );
    glBindBuffer( GL_ARRAY_BUFFER, display_vbo );

    glDrawArrays( GL_TRIANGLES, 0, 6 );


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // show the demo window
    // static bool show_demo_window = true;
    // if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // do my own window
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(256,385));
    ImGui::Begin("Controls", NULL, 0);


    //do any widgets you want here



    ImGui::End();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());   // put imgui data into the framebuffer

    SDL_GL_SwapWindow(window);  // swap the double buffers

    // handle events

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            pquit = true;

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            pquit = true;

        if ((event.type == SDL_KEYUP  && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_X1)) //x1 is browser back on the mouse
            pquit = true;
    }
}


void engine::quit()
{
  //shutdown everything
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  //destroy window
  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  cout << "goodbye." << endl;
}
