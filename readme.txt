Ini adalah project reindeer. Project ini di masa mendatang akan menjadi project 3D game engine. Namun untuk sementara, sekarang project ini masih dipakai sebagai sarana belajar 3d programming.

Reindeer dapat di-render dengan Direct3D dan juga OpenGL. IDE yang digunakan adalah Visual Studio dengan compiler VC++ atau Codeblocks dengan compiler GCC jika menggunakan linux atau MinGW jika menggunakan Windows. Untuk tiap IDE sudah disiapkan file descriptor projectnya (.sln, .vcproj, .workspace, .cbp, dll). Jika Anda tidak berkenan dengan IDE tersebut, tentu saja Anda dapat memakai IDE atau text editor kesukaan Anda.

Dependensi:
- glew http://glew.sourceforge.net/
- glfw3 http://www.glfw.org/
- glm http://glm.g-truc.net/0.9.5/index.html (cuma header)
- bullet physics http://bulletphysics.org/wordpress/

Sebelum mem-build reindeer, silakan download dependensi di atas terlebih dahulu dan build sebagai static library (tidak perlu build apabila sudah disediakan pre-compiled binary sesuai platform), lalu copy file binary-nya ke folder /lib dan file-file header ke folder /include.
Untuk sementara lib bullet physics yang dibutuhkan hanya BulletCollision, BulletDynamics, dan LinearMath.

Progress sekarang: https://www.youtube.com/watch?v=ELnp_dx780g

To do:
- hasil render model object aneh di d3d9
- port direct3d 11
- lighting
- texture
- shadow
- fog
- animation
- editor, kalo bisa sih sekeren unity3d :p

catatan:
- ide baru untuk pertimbangan, pake compositor pattern untuk object. sepertinya unity pake pendekatan ini juga.
- referensi contoh misi sebuah game engine: https://github.com/turbulenz/turbulenz_engine#what-are-the-design-goals-of-the-turbulenz-engine

Terima kasih telah mengunjungi reindeer.