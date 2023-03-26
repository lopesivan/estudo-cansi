Em C, a herança é implementada usando estruturas com campos que representam as classes. As subclasses são implementadas como estruturas com uma estrutura mãe aninhada, permitindo que as classes filhas herdem seus atributos e métodos.

Na implementação acima, a classe mãe abstrata "Mamifero" é definida como uma estrutura que contém um ponteiro para um método abstrato falar(). As classes filhas "Gato" e "Cachorro" são definidas como estruturas que contêm uma estrutura mãe aninhada e seus próprios métodos falar().

Na função main(), instâncias das classes filhas são criadas como variáveis Gato e Cachorro, com seus respectivos métodos falar() atribuídos aos ponteiros de função mamifero.falar. Em seguida, seus métodos falar() são chamados diretamente, produzindo a mesma saída que os exemplos anteriores.

Observe que, em C, não há uma maneira direta de definir classes abstratas. No entanto, estamos simulando a abstração usando ponteiros para funções, que são chamados de "métodos abstratos". Esses métodos precisam ser definidos e atribuídos aos campos corretos nas classes filhas para que possam ser chamados com sucesso.

