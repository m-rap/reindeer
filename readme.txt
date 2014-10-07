Ini adalah project reindeer. Project ini akan menjadi project 3d game engine, tapi masih jauh. Untuk sementara, sekarang project ini masih dipakai untuk project belajar 3d programming.

Ada Directx, ada juga OpenGL. IDE yang disarankan untuk Directx adalah Visual Studio dengan compiler VC++, sedangkan OpenGL menggunakan Codeblocks dengan compiler GCC jika menggunakan linux atau MinGW jika menggunakan Windows. Untuk tiap IDE sudah disiapkan file descriptor projectnya (.sln, .vcproj, .workspace, .cbp, dll).

Dependensi:
- glew http://glew.sourceforge.net/
- glfw3 http://www.glfw.org/
- glm http://glm.g-truc.net/0.9.5/index.html (cuma header)
- bullet physics http://bulletphysics.org/wordpress/

Sebelum mem-build reindeer, silakan download dependensi di atas terlebih dahulu dan build sebagai static library (syukur-syukur kalo sudah disediakan pre-compiled binary sesuai platform), lalu copy file binary-nya ke folder /lib dan file-file header ke folder /include.
Untuk bullet physics sementara hanya butuh build BulletCollision, BulletDynamics, dan LinearMath.

Terima kasih telah mengunjungi reindeer.