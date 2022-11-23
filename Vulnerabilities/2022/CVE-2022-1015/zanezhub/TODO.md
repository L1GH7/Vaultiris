## Aclarar las siguientes dudas:

* [Duda 1 - Traducción](README.md#11-identificando-el-objetivo-y-la-estrategia-de-auditoría)
  
  Si un *bug* requiere privilegios *root*, no existe un límite de seguridad significativo (a menos de que el [*kernel module signing*]([Kernel module signing facility — The Linux Kernel documentation](https://www.kernel.org/doc/html/v5.0/admin-guide/module-signing.html)) esté activado)

* [Traducir parte del código](README.md#41-root)

* [Duda 2 - ¿A qué se refiere el término *chain hook*?](README.md#42-examinando-los-exploit-primitives)

* [Duda 3 - ¿Qué es un *dynamic set*?](README.md#43-filtrado-de-información-de-un-canal-lateral-side-channel)

* [Duda 4 - ¿A qué se refiere *nft_do_chain* runs?](README.md#43-filtrado-de-información-de-un-canal-lateral-side-channel)

* [Duda 5 - Dudas en la traducción con los términos de *base chain* y *auxiliary regular chain*.](README.md#43-filtrado-de-información-de-un-canal-lateral-side-channel)
  
  Todavía existen algunas advertencias. Por ejemplo, el paquete que recibimos también podría ser soltado sin ningún previo aviso. Para mitigar esto, podemos añadir una reducción de ruido, para la cual necesitaremos una *base chain* y una *auxiliary regular chain*.

* [Duda 6 - ¿Qué significa *contrived stack frame hacking*?](README.md#44-ejecución-arbitraria-de-código-arbitrary-code-execution)
  
  Ahora, tal vez podamos hacer un poco de *contrived stack frame hacking*.

* [Duda 7 - ¿Qué significa *verdict chain pointer*?](README.md#44-ejecución-arbitraria-de-código-arbitrary-code-execution)

* [Duda 8 - Duda con la siguiente traducción](README.md#44-ejecución-arbitraria-de-código-arbitrary-code-execution):
  
  ¡Esto se ve un poco mejor! Podemos sobreescribir la dirección de retorno del *frame* de `__netif_receive_skb_one_core` (*offset* `+0x328`), que regresa hacia `__netif_receive_skb`. Ya que está relativamente cerca a la altura del alcance fuera de límites de nuestro `nft_payload`, podemos hacer que nuestro index *OOB* (alcance fuera de límites) apunte directamente a esta dirección de retorno, eludiendo el *stack canary* en el *offset* `+0x310`. El *offset* `+0x328` se traduce al index `0xca`.
  
  That looks a lot better! We can overwrite the return address of the `__netif_receive_skb_one_core` frame (offset `+0x328`), which returns to `__netif_receive_skb`. Because it is still relatively close to the top of our `nft_payload` OOB reach area, we can make our OOB index point to this return address directly, bypassing the stack canary at offset `+0x310`. Offset `+0x328` translates to index `0xca`.
