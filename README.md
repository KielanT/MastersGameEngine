# MastersGameEngine
 
### Install:
    1. Clone the commit using -recusive
        - git clone https://github.com/KielanT/MastersGameEngine.git -recusive
        
    2. Init & update submodules
    
    3. Generate GenProject.bat
    
    4. Build PhysX
        - Go to: MastersGameEngine\Engine\external\physx\physx
        - Run: generate_projects.bat
        - Go to: MastersGameEngine\Engine\external\physx\physx\compiler\vc16win64
        - Open Sln
            - In debug mode:
                - In the PhysX SDK Folder Select all projects
                - Right Click Properties
                - go to code generation 
                - change runtime library from /MTd to /MDd
                - Apply and build
            - In release mode:
                - In the PhysX SDK Folder Select all projects
                - Right Click Properties
                - go to code generation 
                - change runtime library from /MT to /MD
                - Apply and buil
    5. Build SDL
        - Go To: MastersGameEngine\Engine\external\SDL\VisualC
        - Open sln
            - Run in debug and release mode
            
    6. Open MastersGameEngine.sln 
        - Build & Run
        
            
        
