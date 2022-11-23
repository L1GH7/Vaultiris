# CVE-2022-1015 & CVE-2022-1026

Este README.md es una traducción del [blog de David](https://blog.dbouman.nl/2022/04/02/How-The-Tables-Have-Turned-CVE-2022-1015-1016/#1-background). David encontró los CVE's 1015 y 1016 en el kernel de Linux. Puedes visitar su página web para leer el documento original.

Aquí te dejo sus redes sociales:

* [Twitter](https://www.twitter.com/pqlqpql)
* [Github](https://github.com/pqlx)

# Un análisis de las dos nuevas vulnerabilidades de Linux en nf_tables

*Publicado el 2 de abril del 2022.*

* CVE-2022-1015 permite realizar un acceso out-of-bounds (fuera del límites) causado por escasas validaciones de argumentos de entrada, puede derivar en la ejecución de código remoto y a una escalación de privilegios local.
* CVE-2022-1016 está relacionado a una pobre de inicialización de las variables alojadas en el *stack*, lo que puede ser usado para filtrar una larga variedad de datos del kernel al [espacio del usuario (userspace)](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwiem6O-tfb2AhVCnWoFHYkYAnUQFnoECAoQAQ&url=https%3A%2F%2Fes.wikipedia.org%2Fwiki%2FEspacio_de_usuario&usg=AOvVaw3oar6quUITZCdk6kx1oppD).

Estos problemas deberían ser explotabes en las configuraciones por defecto de la versión más nueva de Ubuntu y de [RHEL](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjD3aGNtvb2AhUIlGoFHbSABtUQFnoECAoQAQ&url=https%3A%2F%2Fwww.redhat.com%2Fes%2Ftechnologies%2Flinux-platforms%2Fenterprise-linux&usg=AOvVaw0Q9tKeMmdnMbFfCey0X1d1). Escribí mi prueba de concepto (PoC) del CVE-2022-1015 tomando como objetivo la versión del kernel 5.16-rc3 de Arch Linux.

Este documento está dirigido a las personas que tengan un conocimiento básico del kernel de Linux en términos de funcionalidad y seguridad. Traté de hacer que este documento sea amigable con las personas que carezcan de conocimientos con el *stack* de redes para hacerlo accesible a todo público. 

Aquí está una guía de lectura:

* Si estás aquí simplemente para leer acerca de la vulnerabilidad, empiezan en la Sección 4
* Si también quieres un poco de contexto acerca del subsistema del kernel, empieza con la Sección 2
* Si estás interesado en un poco más de contexto adicional, lee todo el documento

## 1. Contexto

A mediados de febrero, el programa de seguridad de Google anunció que continuarían su [programa de recompensas kCTF](https://security.googleblog.com/2022/02/roses-are-red-violets-are-blue-giving.html), ofreciendo recompensas que llegan desde los $31,337 hasta  91,337 dólares por un exploit en el kernel de Linux que pueda escalar privilegios al usuario root desde procesos sin privilegios en un sandbox de [nsjail](https://github.com/google/nsjail).

Siendo un pobre estudiante, obviamente esto captó mi atención. Esta era mi primera vez buscando buscando una vulnerabilidad del "mundo real", pero en mis aventuras jugando CTF con mi [equipo](https://ctftime.org/team/42934/), me he familiarizado con el kernel de Linux en términos de seguridad. Después de horas y horas con muy poco cercano a nada de progreso (pero con mayor conocimiento acerca de Linux) logré encontrar algunas vulnerabilidades en el módulo de `nf_tables`.

Tristemente, al final del día, me di cuenta de que este módulo no estaba presente en las reglas del kCTF de Google (por lo que no conseguí ninguna recompensa por estas dos vulnerabilidades). Pero obviamente, aún y así las reporté y escribí un exploit LPE (Escalado de Privilegios Local) para el CVE-2022-1015.

### 1.1 Identificando el objetivo y la estrategia de auditoría

Bien, así que has decidido que vas a encontrar algunas vulnerabilidades en Linux. ¿Ahora qué? Linux es un proyecto gigantezco, y es bastante fácil no poder ver el bosque por los árboles (te enfocas tanto en los detalles que pierdes visión de lo que es realmente importante, no tienes una vista general de la situación). Para empeorar las cosas, muchas partes no está documentadas y necesitas leer un montón de código para poder entender lo que está pasando.

Yo comencé intentando tener una perspectiva detallada del modelo de seguridad de Linux. Encontrar un *bug* es una cosa; pero encontrar un buen *bug* es otra muy distinta. Después de todo, no todos los *bugs* están creados igual:

* Si un *bug* requiere privilegios *root*, no existe un límite de seguridad significativo (a menos de que el [*kernel module signing*](https://www.kernel.org/doc/html/v5.0/admin-guide/module-signing.html) esté activado)
  * Algunas cosas que se me vienen a la mente son muchos de los módulos de los sistemas de ficheros (virtuales). Solo el usuario *root* inicial puede montar estos sistemas de ficheros. La excepción recae en [*vfe*](http://www.dit.upm.es/~jantonio/documentos/revistas/vfs/vfs_1.html) que específica `FS_USERNS_MOUNT`, en cuyo caso puedes montarlos en el [*user namespace*](https://man7.org/linux/man-pages/man7/user_namespaces.7.html).
* Si un no se puede acceder a un *bug* a través de las llamadas al sistema, probablemente no podrá ser explotable.
  * Esto aplica a muchos de los drivers de hardware, ya que no tienes acceso físico a la máquina. Los drivers de red de bajo nivel todavía podrían ser un buen objetivo si es que puedes *p. ej.* enviar datos a través de bluetooth o 802.11.ac.
  * Obviamente esto depende del escenario en el que te encuentres.
* Muchos *bugs* requieren `CAP_SYS_ADMIN` o `CAP_NET_ADMIN`.
  * Los *user namespaces* (espacio de nombre) están activados por defecto así que esto no es un problema.
  * De lo contrario primero tendrás que hacer un escalado de privilegios al *namespace* (espacio de nombre) del usuario *root* dentro de un contenedor.
* No todos los módulos estarán presentes en tu objetivo.
  * Linux es un pedazo de software excepcional altamente configurable, por lo que todas las configuraciones pueden variar de una gran multitud de formas.
  * La configuración del kernel usualmente se puede acceder desde `/proc/config.gz`. Los módulos pueden ser cargados en (=m) o compilados por separado y cargados en tiempo de ejecución (=y).
  * Puedes usar `/proc/modules` y `/proc/kallsyms`, pero siempre son confiables, ya que los módulos pueden ser cargados dinámicamente en el kernel (*p. ej.* `request_module`).
  * Si no estás seguro, escribe un pequeño programa que intente interactuar con el módulo.

Estas restricciones nos ayudan a saber los límites de los sistemas de archivos en los cuáles podemos buscar vulnerabilidades. Creo que es una buena idea tomarte tu tiempo tratando de planear tu ataque al objetivo que desees.

Ya he aprendido mi lección acerca del punto anterior. Como mencioné, el módulo `nf_tables` no estaba cargado en la instancia que nos presentó kCTF. Pude haberme dado cuenta de esto desde un principio y ahorrarme la decepción :p. Por otra parte, probablemente no estarías leyendo este blog ahora mismo si me hubiese dado cuenta antes, supongo que las cosas salieron bien después de todo.

Una explicación por la cuál el *COS*, [Google's container-optimazed Linux fork](https://cloud.google.com/container-optimized-os/docs/concepts/features-and-benefits), no tuviera `nf_tables` puede ser encontrada [aquí](https://github.com/kubernetes/kubernetes/issues/45385) y [aquí](https://github.com/kubernetes/kubernetes/issues/96018).

### 1.2 nf_tables: ¿por qué?

Después de evaluar los puntos anteriormente mencionados, decidí que mi mejor ruta para comenzar probablemente sería mirar el código fuente de la red. Muchas de las funcionalidades interesantes allí necesitan `CAP_NET_ADMIN`, pero como lo mencioné, esto en realidad no es un problema. Por el contrario, sospecho que los componentes que requieren capacidades especiales son por lo general menos seguros, ya que los desarrolladores del kernel pueden tener una falsa sensación de seguridad.

También hice el esfuerzo para escoger el sistema de ficheros del cuál quería conocer más; de esta forma, incluso si no encuentras ningún bug aún así podrás aprender un montón de cosas interesantes.

Investigué muchos sistemas de ficheros de red, pero no encontré nada importante. Después de navergar el subdirectorio `net/`, y me encontré con el módulo `nf_tables`. Parecía un poco complejo, así que decidí  tomarme un tiempo para conocer acerca del mismo.

## 2. Introducción a netfilter

Netfilter (`net/netfilter`) es un subsistema de ficheros de red bastante grande en el kernel. En resumen, netfilter coloca *hooks* a través de los módulos de red que otros módulos pueden registrar manejadores. Cuando se alcanza un *hook*, el control es delegado a esos manejadores, y pueden operar con su respectiva estructura de paquetes de red. Los manjeadores pueden aceptar, soltar y modificar paquetes.

---

## 4. CVE-2022-1015

Después de algunas horas de navegar el API de `nf_tables` (`net/netfilter/nf_tables_api.c`) para empezar a saber cómo funciona de una manera exacta, decidí echar un vistazo a la validación lógica a los registros que el usuario manda, y encontré algunos comportamientos sospechosos. Después de pensar si me estaba volviendo loco o no, escribí un pequño PoC (prueba de concepto) para intentar activar la vulnerabilidad que encontré: una vulnerabilidad conocida como *OOB* o fuera de los límites, que permite leer y escribir en la memoria *stack*.

Después de encontrar una manera para filtrar las direcciones del kernel, tomar control del puntero de memoria fue bastante fácil. Después de un poco de [ROP (programación orientada al retorno)](https://en.wikipedia.org/wiki/Return-oriented_programming), y la *shell* con privilegios *root* se convirtió en una realidad.

### 4.1 Root

Cada vez que la rutina `init` de una expresión necesita [parsear](https://es.wikipedia.org/wiki/Analizador_sint%C3%A1ctico) un registro de un mensaje de usuario de netlink, la `nft_parse_register_load` o `nft_parse_register_store` rutina es llamada dependiendo de si es un registro fuente o un registro de destino. Añandí algunos comentarios:

```c
int nft_parse_register_load(const struct nlattr *attr, u8 *sreg, u32 len)
{

    /* Given a netlink attribute and the length
     * that is required to read the requested data,
     * write a register index to `sreg` or return
     * an error on failure. */

    u32 reg;
    int err;


    reg = nft_parse_register(attr);
    err = nft_validate_register_load(reg, len);
    if (err < 0)
        return err;

    /* Write resulting index to the nft_expr.data structure. */
    *sreg = reg;
    return 0;
}

-----

static unsigned int nft_parse_register(const struct nlattr *attr)
{
    /* Convert a register to an index in nft_regs */

    unsigned int reg;

    /* Get specified register from netlink attribute */
    reg = ntohl(nla_get_be32(attr));

    switch (reg) {
    /* If it's 0 to 4 inclusive,      
     * it's an OG 16-byte register and we need to
     * multiply the index by 4 (4*4=16) */
    case NFT_REG_VERDICT...NFT_REG_4:
        return reg * NFT_REG_SIZE / NFT_REG32_SIZE;

    /* Else we subtract 4, since we need to account
     * for the OG registers above. */
    default:
        return reg + NFT_REG_SIZE / NFT_REG32_SIZE - NFT_REG32_00;
    }

    /* So supplied values of 1, 2, 3, 4 map to
     * OG 16-byte registers, with indices 4, 8,
     * 12, 16
     * Supplied values of 5, 6, 7 overlap the verdict,
     * 8,9,10,11   overlap with OG register 1
     * 12,13,14,15 overlap with OG register 2
     * etc. */

}

-----

static int nft_validate_register_load(enum nft_registers reg, unsigned int len)
{
    /* We can never read from the verdict register,
     * so bail out if the index is 0,1,2,3 */
    if (reg < NFT_REG_1 * NFT_REG_SIZE / NFT_REG32_SIZE)
        return -EINVAL;

    /* Invalid operation, bail out */
    if (len == 0)
        return -EINVAL;

    /* If there would be an OOB access whenever
     * `reg` is taken as index and `len` bytes are read,
     * bail out.
     * sizeof_field(struct nft_regs, data) == 0x50 */
    if (reg * NFT_REG32_SIZE + len > sizeof_field(struct nft_regs, data)) 
        return -ERANGE;

    return 0;
}    
```

Las variantes `*_store` son virtualmente idénticas, excepto que permiten escribir al *verbdict* bajo algunas condiciones.

Después de revisar la última validación, algo está realmente fuera de lugar aquí:

```c
if (reg * NFT_REG32_SIZE + len > sizeof_field(struct nft_regs, data))
```

Esto parece ser un *integer overflow*, ¿no lo creen? Si podemos hacer que `reg` contenga algún valor multiplicado multiplicado por 4 que genere un *overflow* cuando se le sume `len`, podemos satisfacer las condiciones. En `nft_parse_register_load`, el último byte valioso de `reg` todavía está escrito al puntero `u8 *sreg`, cayendo en nuestro `nft_expr` que es usando posteriormente como un index.

```c
*sreg = reg;
```

 ¿De verdad podemos? `reg` es un `enum nft_registers` en la validación de la rutina, de todas formas. Podemos pasar valores que tengan un rango entre `0x00000001` hasta `0xfffffffb` inclusive, el rango de `nft_parse_register`; pero ¿será `reg` un valor de 32 bits en `nft_validate_register_load`? Se sabe que los compiladores podrían encoger los *enum types* si un tipo más pequeño puede representar todos los valores. Vamos a obtener una segunda opinión.

Obtenido del manual de GCC:

```
The integer type compatible with each enumerated type (C90 6.5.2.2, C99 and C11 6.7.2.2).
Normally, the type is unsigned int if there are no negative values 
in the enumeration, otherwise int. If -fshort-enums is specified,
then if there are negative values it is the first 
of signed char, short and int that can represent all the values, 
otherwise it is the first of unsigned char, unsigned short and unsigned int 
that can represent all the values.

On some targets, -fshort-enums is the default; this is determined by the ABI.
```

[¿TL;DR?](https://es.wikipedia.org/wiki/TL;DR) Depende del ABI y el posible grado de optimización. No pude encontrar ninguna evidencia concreta de si esta opción está activada por defecto en las builds de Linux.

Pero el ensamblador nunca miente. Vamos a echar un vistazo:

```objdump.x86asm
0000000000001b60 <nft_parse_register_load>:
    1b60:    e8 00 00 00 00           call   1b65 <nft_parse_register_load+0x5>
    1b65:    55                       push   rbp
    1b66:    8b 47 04                 mov    eax,DWORD PTR [rdi+0x4]
    1b69:    0f c8                    bswap  eax
    1b6b:    89 c7                    mov    edi,eax
    1b6d:    8d 48 fc                 lea    ecx,[rax-0x4]
    1b70:    c1 e7 04                 shl    edi,0x4
    1b73:    48 89 e5                 mov    rbp,rsp
    1b76:    c1 ef 02                 shr    edi,0x2
    1b79:    83 f8 04                 cmp    eax,0x4
    1b7c:    89 f8                    mov    eax,edi
    1b7e:    0f 47 c1                 cmova  eax,ecx
    1b81:    85 d2                    test   edx,edx
    1b83:    74 13                    je     1b98 <nft_parse_register_load+0x38>
    1b85:    83 f8 03                 cmp    eax,0x3
    1b88:    76 0e                    jbe    1b98 <nft_parse_register_load+0x38>
    1b8a:    8d 14 82                 lea    edx,[rdx+rax*4]
    1b8d:    83 fa 50                 cmp    edx,0x50
    1b90:    77 0d                    ja     1b9f <nft_parse_register_load+0x3f>
    1b92:    88 06                    mov    BYTE PTR [rsi],al
    1b94:    5d                       pop    rbp
    1b95:    31 c0                    xor    eax,eax
    1b97:    c3                       ret    
    1b98:    b8 ea ff ff ff           mov    eax,0xffffffea
    1b9d:    5d                       pop    rbp
    1b9e:    c3                       ret    
    1b9f:    b8 de ff ff ff           mov    eax,0xffffffde
    1ba4:    5d                       pop    rbp
    1ba5:    c3                       ret    
```

Las llamadas a funciones están alineadas bastante bien. Las operaciones importantes están en `1b8a`:

```objdump.x86asm
lea    edx, [rdx+rax*4]
cmp    edx, 0x50
ja     1b9f <nft_parse_register_load+0x3f>
mov    BYTE PTR [rsi], al
```

`rax` es el resultado de `ntf_parse_register`, `rdx` es `len` proporcionada, y `rsi` es el puntero `sreg`. Ya nos hemos quitado de dudas.

`nft_parse_register_store` muestra el mismo comportamiento. Mientras los registros vivan en el *stack*, nuestra vulnerabilidad *OOB* obviamente será relativa del *stack*. Esto es bueno, porque con un poco de suerte, podremos sobreescribir y retornar memoria directamente.

Para dar un ejemplo de una entrada vulnerable, un registro de `0xfffffffb` y una longitud de `0x20`, va a evaluar `0xfffffffb * 4 + 0x20 = 0x0c < 0x50`. Después de la validación `(u8)0xfffffffb = 0xfb` será escrito a `*sreg`.

Aunque hay un problema: ¿existen expresiones que nos permitan usar una longitud que pueda causar un *overflow* cuando se realice la suma? Después de un poco de investigación, encontré que `nft_bitwise` y `nft_payload` te permiten dar como entrada tu propia longitud, desde `0x00` hasta `0xff`. Muchas otras expresiones parecen tener longitudes estáticas que son muy pequeñas.

De momento esto se ve prometedor. El siguiente paso es tomar estos *exploit primitives* (capacidad génerica ganada durante un *exploit*) y usarlos.

### 4.2 Examinando los *exploit primitives*

Si podemos definar el tipo de poder que nos puede dar nuestro *exploit*, explotar esta vulnerabilidad debería ser más fácil. Así que, denme un poco de su paciencia porque vamos a ver un poco de aritmética.

Hay tres puntos que podemos usar para nuestro *overflow* para la multiplicación de registro, ya que esto es multiplicado por `4 = 2^2`: `2^32 - 1`, `2^31 - 1` y `2^30 - 1` (respectivamente `0xffffffff`, `0x7fffffff`, y `0x3fffffff`). Estos valores pueden ir decreciendo hasta que sumemos nuestra máxima longitud permitida, después de ser multiplicada por cuatro esto no resultará en un *overflow*. Otro punto a tomar en cuenta es que no podemos usar valores mayores a `0xfffffffb`, como se mencionó con anterioridad.

Dando una longitud específica, los valores byte menos significativos que pueden permitir un *overflow* usando esta longitud formarán nuestro intervalo de índices OOB que podemos usar.

Después de todo, no importa qué puntos de *overflow* sean usados. Toma por ejemplo los siguientes valores con un [LSB (bit menos significativo)](https://es.wikipedia.org/wiki/Bit_menos_significativo) de `0xf0`:

```
0xfffffff0 * 4 = 0xffffffc0
0x7ffffff0 * 4 = 0xffffffc0
0x3ffffff0 * 4 = 0xffffffc0
```

De ahora en adelante, vamos a usar valores de registro cercanos a `0x7fffffff`.

Anteriormente hemos dado hablado de `nft_payload` and `nft_bitwise`. Algunas propiedades de estas expresiones son:

* `nft_payload` solo puede realizar escrituras *OOB*, mientras que `nft_bitwise` puede realizar escrituras y lecturas *OOB*.

* `nft_payload` puede hacer escrituras *OOB* hasta 0xff bytes de datos arbitrarios.

* `nft_bitwise` realmente solo puede escribir hasta `0x40` bytes de datos arbitrarios y puede leer solo `0x40` bytes de datos que se encuentren en el *stack* del espacio del registrador .
  
  * `nft_bitwise` requiere un `sreg` y un `dreg`, los cuáles necesitan pasar la validación con el mismo valor de longitud.
  
  * Solo tenemos `0x40` bytes de espacio de registro, así que queremos o leer o escribir del registro de espacio, pero no podemos pasar la validación con una longitud mayor a `0x40`.

Podemos usar un valor de longitud más grande para `nft_bitwise`, pero significa que `sreg` y `dreg` necesitan estar fuera de los límites, lo cuál no sería muy útil para nuestros propósitos. Así que, por ahora trabajaremos con la longitud de `0x40`.

Teniendo todo esto en cuenta, ¿qué tipos de *exploits* podremos usar?

`nft_bitwise` tiene una longitud máxima de `0x40`. Esto significa que el valor de registro multiplicado por cuatro debería ser al menos `0xffffffc0`. El valor más grande que podemos obtener multiplicando por cuatro es `0xfffffffb`, y ya que `0xfffffffb + 0x40 = 0x3b <= 0x50` esto pasará la validación.

`0x7ffffff0 * 4 = 0xffffffc0`: el límite inferior es `0xf0`.
`0x7fffffff * 4 = 0xfffffffb`: el límite superior es `0xff`.

Traduciendo a [*byte offsets*](https://es.wikipedia.org/wiki/Offset_(inform%C3%A1tica)):

```
0xc1 * 4        = 0x304
0xeb * 4 + 0xff = 0x4ab
```

`nft_payload` puede escribir fuera de límites a través de los *offsets* `[0x304, 0x4ab]` desde `struct nft_regs`.

Ahora que todo esto ya está aclarado, ¿qué es lo que en realidad está en el *stack* en estos *offsets*?

La rutina `nft_do_chain` puede ser llamada a través de muchas rutas de código. Existen muchos factores que cambiarán la forma del *stack* antes del [*stack frame* (marco de *stack*)](https://es.wikipedia.org/wiki/Pila_de_llamadas) de `nft_do_chain`:

* Ya sea que el *chain hook* sea un `input` o `output`.
  
  * Si tenemos un *chain hook* configurado como `input`, el *hook*  se activará en el contexto [*softirq*](https://programmerclick.com/article/4044193708/) del dispositivo de red respectivo con el *stack* softirq.
  * Si tenemos un *chain hook* configurado como `output`, el *hook* se activará en el contexto de [*syscall* (llamada al sistema)]([https://www.ionos.mx/digitalguide/servidores/know-how/que-son-las-system-calls-de-linux/) `send*` con el *stack* de *syscall*.

* El protocolo que estamos usando.
  
  * Mandar un paquete IP bruto tendrá un [*call stack*](https://es.wikipedia.org/wiki/Pila_de_llamadas) bastante diferente a *p. ej.* un paquete UDP. 

Creo que puedes obtener una muchas variaciones de *call stacks* usando diferentes combinaciones de protocolos, interfaces y localicaciones de *hooks*. Por el momento estaremos usando un *chain hook* configurado como un `output` con un paquete UDP.

![Diagrama del stac con output y UDP](imgs/1-stack_layout_output_udp.png)

*Diseño del stack y los alcances fuera de límites en nft_do_chain cuando un paquete UDP enviado alcanza un hook configurado a output*

### 4.3 [Filtrado de información de un canal lateral (*side-channel*)](https://es.wikipedia.org/wiki/Ataque_de_canal_lateral)

Para poder crear un exploit estable primero tendremos que filtrar la dirección de la imagen del kernel.

La dirección de la imagen del kernel tiene 9 bits de entropía (medida de la incertidumbre existente ante un conjunto de mensajes, del cual va a recibirse uno solo), lo que significa que hay 512 diferentes posiciones en las cuales el kernel puede ser cargado. Dependiendo del escenario de tu ataque, hay una probabilidad de 1 en 512 de conseguir que el ataque funcione correctamente; pero sería mejor si pudiésemos conseguir un exploit más estable de esto.

El paso más sencillo es tratar de usar nuestra capacidad de lectura fuera de límites que nos consiguió `nft_bitwise` para copiar algunos de los datos del *stack* a nuestros registros. Ya que el intervalo total que podemos leer tiene una longitud de`0x7c` bytes, existe una probabilidad bastante buena de que la dirección del kernel esté ahí.

![Alcance fuera de límites de nft_bitwise](imgs/2-nft_bitwise_oob_reach.png)

*Alcance fuera de límites de nft_bitwise*

¡Hoy es nuestro día! Existen dos:

```
gef➤  x/bx 0xffffffff815b49c1
0xffffffff815b49c1 <import_iovec+49>:    0xc9
gef➤  x/bx 0xffffffff819ac3ec
0xffffffff819ac3ec <copy_msghdr_from_user+92>:    0xba
```

Escribir esto a los registros es una cosa, pero extraerlos es otra. Después de investigar, parece que no existe una forma fácil para leer directamente los registros cuando `nft_do_chain` está en ejecución.

En mi reporte original a security@k.o, me informaron de que la expresión `nft_dynset` por un mantenedor de netfilter, que tiene soporte para [*dynamic sets*](https://en.wikipedia.org/wiki/Dynamic_set) que pueden actuar como una especie de base de datos que puede escribir y leer a través de diferentes `nft_do_chain` ejecuciones. Aparentemente, el `nft_payload` también tiene la capacidad de escribir al paquete por sí mismo, no me di cuenta de esto.

En su lugar, decidí continuar con mi [*side-channel attack*](https://es.wikipedia.org/wiki/Ataque_de_canal_lateral). Debido a la naturaleza de `nf_tables`, puedes causar efectos secundarios. De hecho, podrías decir que ni siquiera son efectos secundarios, sino efectos primarios.

Creando reglas que sueltan o aceptan el paquete basándose en el valor de la dirección de memoria del kernel que estamos copiando, poco a poco podemos deducir cuál es el valor examinando si los paquetes que enviamos también fueron recibidos.

1. Crear un *socket* UDP que recibe paquetes en `127.0.0.1:9999`:
* Debería recibir paquetes en un hilo diferente.

* Un mensaje debería ser enviado de vuelta por cada paquete que reciba.
2. Agrega una regla que:
   
   1. Copia la dirección del kernel a los registros con `nft_bitwise`.
   
   2. Usa `nft_cmp_expr` para comparar la dirección a una constante.
   
   3. Soltar un paquete si la comparación evaluada es verdadera.

3. Envía un paquete UDP a `127.0.0.1:9999`
   
   1. Podemos determinar un poco de información acerca de la dirección del kernel basado en si recibimos un mensaje de vuelta.

4. Repite 2 y 3 con los valores adecuados hasta que tienes suficiente información para determinar la información por sí sola.

![](imgs/3-please_learn_me_how_to_make_graphics.png)

Todavía existen algunas advertencias. Por ejemplo, el paquete que recibimos también podría ser soltado sin ningún previo aviso. Para mitigar esto, podemos añadir una reducción de ruido, para la cual necesitaremos una *base chain* y una *auxiliary regular chain*.

*Rule in base chain:*

| #   | Expresión            | Argumentos                                                                                               | Comentario                                                                                                 |
| --- | -------------------- | -------------------------------------------------------------------------------------------------------- |:---------------------------------------------------------------------------------------------------------- |
| 0   | `nft_payload`        | base=NFT_PAYLOAD_TRANSPORT_HEADER<br/>offset=offsetof(udphdr, dport)<br/>len=sizeof_field(udphdr, dport) | Escribir el puerto de destino del paquete al registro 8.                                                   |
| 1   | `nft_cmp_expr`       | op=NFT_CMP_EQ<br/>sreg=8<br/>data=9999                                                                   | Equiparar el puerto destino a `9999`, y regresar `NFT_BREAK` si el resultado no es igual.                  |
| 2   | `nft_payload`        | base=NFT_PAYLOAD_INNER_HEADER<br/>offset=0<br/>len=8                                                     | Escribir los primeros ocho bytes del paquete al registro 8.                                                |
| 3   | `nft_cmp_expr`       | op=NFT_CMP_EQ<br/>sreg=8<br/>data=0xdeadbeef0badc0de                                                     | Comparar los primeros ocho bytes al valor mágico, y regresar `NFT_BREAK` si no es igual.                   |
| 4   | `nft_immediate_expr` | verdict=NFT_JUMP<br/>chain=aux_chain                                                                     | Ya que la regla aún está evaluando, las condiciones deben coincidir, y llamar a nuestra *auxiliary chain*. |

*Rule in auxiliary chain:*

| #   | Expresión       | Argumentos                                                              | Comentario                                                                                                                                                                                  |
| --- | --------------- | ----------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0   | `nft_bitwise`   | op=NFT_BITWISE_RSHIFT<br/>data=SHIFT_AMT<br/>dreg=OOB_OFFSET<br/>sreg=8 | Escribe la dirección del kernel a los registros usando la lectura fuera de límites, cambiado por los bits de `SHIFT_AMT` para obtener el byte de la dirección deseada al registro correcto. |
| 1   | `nft_cmp`       | op=NFT_CMP_GT<br/>sreg=ADDRESS_OFFSET<br/>data=COMPARAND                | Comparar los byte de la dirección del kernel con `COMPARAND`, regresar `NFT_BREAK` si este resultado no es igual.                                                                           |
| 2   | `nft_immediate` | verdict=NFT_DROP                                                        | Soltar el paquete si la dirección byte es más grande que `COMPARAND`.                                                                                                                       |

Revisando el puerto destino y comparando los primeros ocho bytes interiores del *header* a un valor mágico, podemos activar los efectos secundarios para los paquetes que queramos.

Cambiando dinámicamente `COMPARAND` podemos hacer una búsqueda binaria para encontrar el byte de la dirección del kernel por la `0(log(n))` vez. Cambiando dinámicamente `SHIFT_AMT` a los próximos múltiplos de ocho podemos movernos al próximo byte de memoria y empezar de nuevo.

#### 4.3.1 Filtrar pseudo-código

Un poco de código en python para filtrar la dirección de memoria. Lo gracioso es que fácilmente pude haber implementado esto en python. Recuerden que no siempre tienen que hacer sus exploits para una kernel en C :p

```python
'''
Asumimos que un hilo secundario está recibiendo
paquetes UDP en 127.0.0.1:9999 y todo lo relacionado
con nf_tables ya está configurado
p. ej. table, base y auxiliary chain
'''

def leak_byte(pos):
    s = socket.socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
    s.settimeout(200) # 200ms debería ser más que suficiente
    s.bind(("127.0.0.1", 1234))

    # buscar los límites
    low = 0, high = 255

    while True:
        mid = (low + high) // 2

        # si encontramos el valor, lo regresamos 
        if low == high:
            s.close()
            return mid

        set_leak_rule(SHIFT_AMT=pos*8, COMPARAND=mid)

        # Enviar el paquete y activar la auxiliary chain
        s.sendto(pack(0xdeadbeef0badc0de), ("127.0.0.1", 9999))

        # El hilo secundario regresa a 127.0.0.1:1234
        res = s.recvfrom(0x2000)

        if not res:
            '''
            nuestro paquete fue soltado
            ya que no se regresó nada en los 200ms
            lo que significa que 

            byte to leak >= mid
            el byte a filtrar es mayor o igual a mid (127)
            '''
            low = mid
        else:
            '''
            [sanity check o prueba de cordura]

            se usa para evaluar rápidamente si 
            el valor a calcular es siquiera posible

            https://es.wikipedia.org/wiki/Prueba_de_cordura
            '''

            if res != b"MSG_OK":
                print("Something went wrong")
                return None

            '''
            Nuestro paquete fue aceptado, lo que
            significa que 

            byte to leak < mid
            byte a filtrar es menor a mid (127)
            '''
            high = mid - 1

leak_bytes = lambda: [leak_byte(i*8) for i in range(4)]
```

### 4.4 [Ejecución arbitraria de código (*Arbitrary code execution*)](https://es.wikipedia.org/wiki/Ejecuci%C3%B3n_arbitraria_de_c%C3%B3digo)

Ahora que conseguimos el leak, la ejecución arbitraria de código debería ser muy fácil. La escritura fuera de límites de `nft_payload` debería de poder escribir un ataque [*RoP*](https://es.wikipedia.org/wiki/Programaci%C3%B3n_orientada_al_retorno) en cadena para el *stack*, ¿cierto?

Nop. No tuvimos mucha suerte, al menos en este kernel en particular. La escritura fuera de límites de `nft_payload` casi en su totalidad se alinea con el *stack frame* de la rutina de `udp_sendmsg`. La dirección de `udp_sendmsg` se encuentra en el *offset* `+0x2f8` relativo a los registros, esta localización es muy baja como para ser alcanzada con `nft_payload` o `nft_bitwise` (podemos comenzar a escribir comenzando en el *offset* `+0x304`, tan cerca...). La dirección `inet_sendmsg` está localizada en el *offset* `+0x4a8`. Técnicamente podemos alcanzarlo (y sobreescribir los tres bytes inferiores), pero hay un [*stack canary*](https://en.wikipedia.org/wiki/Stack_buffer_overflow) (ténica utilizada para la detección de un *stack buffer overflow* antes de que la ejecución de código malicioso pueda suceder) en la dirección `+0x0458` que también necesitamos sobreescribir para lograr esto. Esto obviamente haría que el kernel crasheara, así que hacer esto no es una opción.

Logré usar este método en otra build del kernel, pero parece ser que tratar de hacer lo mismo para la kernel que estoy utilizando para este blog será un poco más difícil.

Ahora, tal vez podamos hacer un poco de *contrived stack frame hacking* para sobreescribir las variables locales en `udp_sendmsg`. También podríamos intentar sobreescribir el *verdict chain pointer*, usando un valor de los registros *p. ej.* `0x7fffff00` (creo que esto podría ser una técnica genial; tomando en cuenta el reto).

Vamos a intentar cambiando la *base chain hook* que usamos. Estábamos usando una *chain* `output`, ¿qué pasaría si la cambiamos a una de `input`

![](imgs/4-stack_layout_input_udp.png)

*El diagrama del alcance fuera de límites en nft_do_chain si un paquete UDP enviado alcanza el input hook*

¡Esto se ve un poco mejor! Podemos sobreescribir la dirección de retorno del *frame* de `__netif_receive_skb_one_core` (*offset* `+0x328`), que regresa hacia `__netif_receive_skb`. Ya que está relativamente cerca a la altura del alcance fuera de límites de nuestro `nft_payload`, podemos hacer que nuestro index *OOB* (alcance fuera de límites) apunte directamente a esta dirección de retorno, eludiendo el *stack canary* en el *offset* `+0x310`. El *offset* `+0x328` se traduce a index `0xca`.

Para activar la sobreescritura de la dirección de regreso, creamos un nuevo `input` *chain* en la tabla, y le añadimos una regla con un `nft_payload` que escribe `0xff` bytes desde el header interior del paquete hacia index `0xca`. Después mandamos un paquete con el *payload*, y boom.
![](imgs/5-rip_control_thankgod.png)

🥳 🥳 🥳 🥳 🥳
