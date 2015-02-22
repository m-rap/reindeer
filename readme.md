REINDEER
======

Ini adalah project reindeer. Project ini di masa mendatang akan menjadi project 3D game engine. Namun untuk sementara, sekarang project ini masih dipakai sebagai sarana belajar 3d programming.

Reindeer dapat di-render dengan Direct3D dan juga OpenGL. IDE yang digunakan adalah Visual Studio dengan compiler VC++ atau Codeblocks dengan compiler GCC jika menggunakan linux atau MinGW jika menggunakan Windows. Untuk tiap IDE sudah disiapkan file descriptor projectnya (.sln, .vcproj, .workspace, .cbp, dll). Jika Anda tidak berkenan dengan IDE tersebut, tentu saja Anda dapat memakai IDE atau text editor kesukaan Anda.

Dependensi
--------

Library:
- [bullet physics](http://bulletphysics.org/)
- Untuk render dengan OpenGL :
  - [glew](http://glew.sourceforge.net/)
  - [glfw3](http://www.glfw.org/)
  - [glm](http://glm.g-truc.net/0.9.5/index.html) (cuma header)
- Untuk render dengan Direct3D :
  - [directx sdk](http://www.microsoft.com/en-us/download/details.aspx?id=8109)

*) Library di atas (selain direct3d) akan di-download secara automatis apabila mem-build menggunakan perintah make

Untuk Linux secara umum:
- wget
- cmake
- rsync
- unzip

Untuk Distro keluarga debian:
- libxxf86vm-dev
- libgl1-mesa-dev
- libxrandr-dev
- xorg-dev
- libglu1-mesa-dev

Building
-------

Di Linux Anda dapat langsung mem-build dengan menjalankan perintah make dari root directory. Namun apabila tidak memungkinkan (misal karena Anda menggunakan Windows) lakukan langkah berikut sebelum mem-build:
- download dependensi library di atas terlebih dahulu 
- build sebagai static library (tidak perlu build apabila sudah disediakan pre-compiled binary sesuai platform)
- copy file binary-nya ke folder /lib dan file-file header ke folder /include.
- install DirectX SDK apabila Anda menggunakan Direct3D

*) Untuk sementara library bullet physics yang dibutuhkan hanya BulletCollision, BulletDynamics, dan LinearMath.

Progress kami
--------
Lihat di [reindeer](https://www.youtube.com/watch?v=ELnp_dx780g)


To do
------
- port direct3d 11
- lighting
- texture
- shadow
- fog
- animation
- editor, contoh yang keren: unity3d, paradox3d

Catatan
------
- Ide baru untuk pertimbangan, menggunakan compositor pattern untuk object, sepertinya Unity pake pendekatan ini juga.
- Referensi contoh misi sebuah game engine: [referensi](https://github.com/turbulenz/turbulenz_engine#what-are-the-design-goals-of-the-turbulenz-engine)


Terima kasih telah mengunjungi reindeer.
