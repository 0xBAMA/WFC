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



    cout << endl << endl;

    m.parse_input();

    cout << endl << endl;

    
    
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
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_RECTANGLE, display_texture);
    // glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8UI, m.in.width, m.in.height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &m.in.data[0]);
    // glBindImageTexture(0, display_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8UI);
}


//  ╦┌┬┐┌─┐┌─┐┌─┐
//  ║│││├─┤│ ┬├┤ 
//  ╩┴ ┴┴ ┴└─┘└─┘
unsigned image::load(std::string filename)
{
    return lodepng::decode(data, width, height, filename.c_str());
}

int image::at(int x, int y) // assume bounds checking is done outside of this function
{
    // if the pixel is off the image
    if(x >= (int)width || x < 0 || y >= (int)height || y < 0)
        return 0; // represents 'INVALID'

    // get the pixel from the image
    int index = (y * width + x) * 4;
    glm::ivec3 select = glm::ivec3(data[index], data[index+1], data[index+2]);
    
    // check against palette, return index of matching color
    for(unsigned i = 1; i < colors.size(); i++)
        if(select == colors[i])
            return i;

    return 0;  // shouldn't see this happen, but have to include for the compiler
}

void image::count_colors()
{
    // index 0 is reserved for 'INVALID'
    colors.resize(1, glm::ivec3(-1));

    // prime the pump with the first color (because of how the inner for-loop below operates)
    colors.push_back(glm::ivec3(data[0], data[1], data[2]));

    // get the rest of the colors in the data array
    for(unsigned i = 4; i < data.size(); i+=4)
    {
        glm::ivec3 current(data[i], data[i+1], data[i+2]); // grab RGB, ignore alpha
        bool seen = false;
        
        for(unsigned j = 1; j < colors.size(); j++)
            if(current == colors[j])
                seen = true; // we have already seen this color

        if(!seen)
            colors.push_back(current); // this is a new color
    }
}

//  ╦═╗┬ ┬┬  ┌─┐
//  ╠╦╝│ ││  ├┤ 
//  ╩╚═└─┘┴─┘└─┘



//  ╔╦╗┌─┐┌┬┐┌─┐┬  
//  ║║║│ │ ││├┤ │  
//  ╩ ╩└─┘─┴┘└─┘┴─┘
void model::parse_input()
{
    // load image, of index F
    std::string filename = std::string("input_samples/" + std::to_string(F) + ".png");
    unsigned error;

    if((error = in.load(filename)))
        cout << endl << "decode error: " << lodepng_error_text(error);
    else
        cout << endl << "Image (" << filename << ") loaded. Width: " << in.width << "  Height: " << in.height << endl;

    // count the colors present in the input image
    in.count_colors();

    // get all the tiles
    pattern temp;
    temp.count = 0;

    // start a timer (establish initial time)
    auto t1 = std::chrono::high_resolution_clock::now();
    std::stringstream percent_done; 
    
    for(int x = 0; x < (int)in.width; x++)
    {
        // clear percentage reporting
        percent_done.str(std::string());
        // report new percentage
        percent_done << "Image " << (int)(((float)x/(float)in.width)*100) << " percent parsed. " << std::flush;

        for(int y = 0; y < (int)in.height; y++)
        {
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    temp.data[i][j] = in.at(x+i, y+j);
                }
            }
            
            switch(SYMMETRY)
            {
                case 8: patterns.push_back(temp.mirror().rotate().rotate().rotate()); // mirror, three ccw rotations
                case 7: patterns.push_back(temp.mirror().rotate().rotate());         // mirror, two ccw rotations
                case 6: patterns.push_back(temp.mirror().rotate());                 // mirror, one ccw rotation
                case 5: patterns.push_back(temp.mirror());                         // mirrored original
                case 4: patterns.push_back(temp.rotate().rotate().rotate());      // three ccw rotations
                case 3: patterns.push_back(temp.rotate().rotate());              // two ccw rotations
                case 2: patterns.push_back(temp.rotate());                      // one ccw rotation
                case 1: patterns.push_back(temp);                              // original
                    break;
            }
        }
        std::cout << "\r" << percent_done.str() << "Distinct tile count: " << patterns.size() << std::flush;
    }

    // end point for the timer
    auto t2 = std::chrono::high_resolution_clock::now();
    cout << "\rParsing complete. Found " << patterns.size() << " tiles in "
         << std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " milliseconds." << endl; 


    
    t1 = std::chrono::high_resolution_clock::now();
    // sort the tiles - uses default, < operator, to sort the list of tiles by the number of occurrences
    std::sort(patterns.begin(), patterns.end());
    
    // I want the largest count first, not last
    std::reverse(patterns.begin(), patterns.end());
    
    t2 = std::chrono::high_resolution_clock::now();
    cout << "Tiles sorted in " << std::chrono::duration_cast<std::chrono::milliseconds>( t2-t1 ).count() << " milliseconds." << endl;


        
    t1 = std::chrono::high_resolution_clock::now();
    // remove duplicate tiles
    int orig = patterns.size();
    // operating on a sorted list, this should be faster
    for(int i = 0; i < (int)patterns.size(); i++)
    {
        for(int j = i+1; j < (int)patterns.size(); j++)
        {
            if(patterns[i] == patterns[j])
            {
                patterns[i].count++;
                patterns.erase(patterns.begin()+j);
                j--;
                continue;
            }
        }
        cout << "\rDuplicate removal: " << ((float)i/(float)patterns.size())*100.0 << "% complete (" << orig-patterns.size() << " tiles removed)." << std::flush;
    }
    
    t2 = std::chrono::high_resolution_clock::now();
    cout << "\rDuplicates removed in " << std::chrono::duration_cast<std::chrono::milliseconds>( t2-t1 ).count() << " milliseconds. "
         << patterns.size() << " tiles remaining. (" << orig-patterns.size() << " tiles removed)"<< endl;

    

    t1 = std::chrono::high_resolution_clock::now();
    // tile dump
    filename = std::string("tiles" + std::to_string(F) + ".png");
    tile_dump(filename);

    t2 = std::chrono::high_resolution_clock::now();
    cout << "Tile dump completed in " << std::chrono::duration_cast<std::chrono::milliseconds>( t2-t1 ).count() << " milliseconds." << endl;
    
    
    
    t1 = std::chrono::high_resolution_clock::now();
    // establish overlap rules

    for(int i = 0; i < (int)patterns.size(); i++) // for each pattern
    {
        for(int x = -N + 1; x < N; x++)
        {
            for(int y = -N + 1; y < N; y++)
            {
                rule r;
                r.offset = glm::ivec2(x, y);
                r.agrees.resize(0);

                for(int j = 0; j < (int)patterns.size(); j++) // for each pattern
                {
                    if(patterns[i].agrees(r.offset, patterns[j]))
                    {
                        r.agrees.push_back(j); // pattern matches, keep it
                    }
                }
                patterns[i].overlap_rules.push_back(r); // all agreeing rules
            }
        }
        cout << "\rOverlap rules " << ((float)i/(float)patterns.size())*100. << "%";
    }
    
    t2 = std::chrono::high_resolution_clock::now();
    cout << "\rOverlap Rules established in " << std::chrono::duration_cast<std::chrono::milliseconds>( t2-t1 ).count() << " milliseconds." << endl;



    // t1 = std::chrono::high_resolution_clock::now();
    // rule dump (JSON)

    json_dump(std::to_string(F) + ".json");
    
    // t2 = std::chrono::high_resolution_clock::now();
    // cout << "Rule dump completed in " << std::chrono::duration_cast<std::chrono::milliseconds>( t2-t1 ).count() << " milliseconds." << endl;

}


void model::tile_dump(std::string filename)
{
    // creates an output image of all parsed tiles, in the order that they appear in the tiles vector
    int num = 150; // how many tiles per row e.g. <tile 0> <tile 1> ... <tile n-1> then a new row starting w <tile n>

    int height, width;
    width = ((N+1) * num) + 1;
    height = 0;
   
    std::vector<unsigned char> image_data;
    image_data.resize(0);

    for(int offset = 0; offset < (int)patterns.size(); offset += num) // rows of tiles
    {
        // row of transparent pixels above these tiles (first row in image, then divider between each)
        for(int i = 0; i < (4*width); i++) // four pixels per tile including transparent leader, four components, plus the final transparent pixel 
            image_data.push_back(0);

        for(int i = 0; i < N; i++) // the patterns themselves are N pixels square, these are slices
        {
            for(int j = 0; j < num; j++) // iterating across the width of the image
            {
                for(int k = 0; k < 4; k++) // first pixel is all zeroes
                    image_data.push_back(0);
               
                if(offset+j < (int)patterns.size())
                {
                    for(int k = 0; k < N; k++) // iterating through the width of a single pattern
                    {
                        image_data.push_back(in.colors[patterns[offset+j].data[i][k]].x);
                        image_data.push_back(in.colors[patterns[offset+j].data[i][k]].y);
                        image_data.push_back(in.colors[patterns[offset+j].data[i][k]].z);
                        image_data.push_back(255);
                    }
                }
                else
                {
                    for(int k = 0; k < (4*N); k++) // a stripe representing a pattern's width
                        image_data.push_back(0);
                }
            }

            //terminate row with another transparent pixel
            for(int j = 0; j < 4; j++)
                image_data.push_back(0);
        }

        height += (N+1);
    }

    height++; // last row of transparent pixels 
    for(int i = 0; i < (4*width); i++)
        image_data.push_back(0);

    unsigned error = lodepng::encode(filename.c_str(), image_data, width, height);
    if(error) cout << endl << "encode error: " << lodepng_error_text(error);    
}

void model::json_dump(std::string filename)
{
    json j;

    // construct the dump

    // number of colors
    j["N"] = N;
    j["colors"]["num"] = in.colors.size();
    
    // indexed colors
    for(int i = 0; i < (int)in.colors.size(); i++)
        j["colors"][std::to_string(i)] = {in.colors[i].x, in.colors[i].y, in.colors[i].z};
        
    // tiles:
    j["tiles"]["count"] = patterns.size();

    for(int i = 0; i < (int)patterns.size(); i++)
    {
        j["tiles"][std::to_string(i)]["count"] = patterns[i].count;
        j["tiles"][std::to_string(i)]["contents"] = patterns[i].data;
        for(auto r : patterns[i].overlap_rules)
        {
            for(int k = 0; k < (int)r.agrees.size(); k++)
            {
                j["tiles"][std::to_string(i)]["agreement[" + std::to_string(r.offset.x) + "][" + std::to_string(r.offset.y) + "]"][std::to_string(k)] = r.agrees[k]; // the offset 
            }
            
        }
    }
    //    integer identifier,
    //    count,
    //    contents using palette index,
    //    [offsets, agreeing tiles (integer identifiers)]
    
    std::ofstream ofile(filename);
    // ofile << j.dump() << std::endl;
    ofile << j.dump(2) << std::endl;
    ofile.close();
}

//  ╔═╗┌─┐┌┬┐┌┬┐┌─┐┬─┐┌┐┌
//  ╠═╝├─┤ │  │ ├┤ ├┬┘│││
//  ╩  ┴ ┴ ┴  ┴ └─┘┴└─┘└┘
bool pattern::agrees(glm::ivec2 offset, pattern &other)
{
    for(int x = 0; x < N; x++)
    {
        for(int y = 0; y < N; y++)
        {
            if(!subagrees(offset, glm::ivec2(x,y), other))
            {
                return false;
            }
        }
    }
    return true;
}


bool pattern::subagrees(glm::ivec2 offset, glm::ivec2 pos, pattern &other)
{
    glm::ivec2 check_pos = pos - offset;

    if((pos.x < 0 || pos.x >= N || pos.y < 0 || pos.y >= N) || (check_pos.x < 0 || check_pos.x >= N || check_pos.y < 0 || check_pos.y >= N)) // either offset is out of bounds (they don't overlap here)
    {
        return true;
    }

    // do the same thing if you have a zero there (invalid palette index means incomplete relationship)
    
    return data[pos.x][pos.y] == other.data[check_pos.x][check_pos.y];
}


pattern pattern::rotate()
{
    pattern temp;

    for(int x = 0; x < N; x++)
    {
        for(int y = 0; y < N; y++)
        {
            temp.data[x][y] = data[(N-1)-y][x];
        }
    }

    return temp;
}

pattern pattern::mirror()
{
    pattern temp;

    for(int x = 0; x < N; x++)
    {
        for(int y = 0; y < N; y++)
        {
            temp.data[x][y] = data[(N-1)-x][y];
        }
    }
    
    return temp;
}




//  ╔═╗┬ ┬┌┬┐┌─┐┬ ┬┌┬┐  ╔╦╗┬┬  ┌─┐
//  ║ ║│ │ │ ├─┘│ │ │    ║ ││  ├┤ 
//  ╚═╝└─┘ ┴ ┴  └─┘ ┴    ╩ ┴┴─┘└─┘






//  ╦ ╦╔═╗╔═╗
//  ║║║╠╣ ║  
//  ╚╩╝╚  ╚═╝



 
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
