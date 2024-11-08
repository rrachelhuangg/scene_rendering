Extension of [Sphere Rendering via Ray Tracing](https://github.com/rrachelhuangg/pixel_sphere_rendering) with significant refactoring and renders complete image scenes with spheres and planes
- Implemented arbitrary aspect ratio so that images are preserved when image dimensions are modified
- Implemented light attenuation: the light loses power as the distance between it and the rendered objects increases
- Implemented shadows
- Implemented planes and checkerboard patterns
- Implemented new SCENE_T data type that contains the rendered objects, the light source, and the pixel size
- Implemented polymorphic object type OBJ_T
- Implemented dynamically allocated objects array in memory: linked list that allocates memory with each object node
- sample scene:
<img width="675" alt="Screenshot 2024-11-07 at 9 24 00 PM" src="https://github.com/user-attachments/assets/a9b9839a-9659-4d11-ac76-997c8b0b429c">
<br/>
- creative scene:
<br/>
<img width="675" alt="Screenshot 2024-11-07 at 9 24 50 PM" src="https://github.com/user-attachments/assets/907a4c36-7b0c-46e4-aa2a-12935affc113">



