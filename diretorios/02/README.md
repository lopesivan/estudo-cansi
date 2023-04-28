Crie um programa em C que rode no linux, e esse programa deve
criar os diretórios  "a/b/c" e os diretórios "a/d/e", dentro de c
deve conter um arquivo chamado "file1.txt" e deve criar um link
simbólico entre "file1.txt" e "file2.txt" que deve se encontrar no
diretório.

```{bash}
a
├── b
│   └── c
│       └── file1.txt
└── d
    └── e
        └── file2.txt -> ../../b/c/file1.txt

4 directories, 2 files
```

