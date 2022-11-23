# HomeKit Denial of Service (CVE-2022-22588)
Patched in iOS 15.2.1 - https://support.apple.com/en-us/HT213043

Esse aplicativo aciona um bug de negação de serviço que pode resultar na perda de funcionalidade de um dispositivo iOS, persistente por meio de reinicializações e acionado novamente ao fazer login novamente no iCloud. [Read more here](https://trevorspiniolas.com/doorlock/doorlock.html)


# AVISO LEGAL
** NÃO execute este aplicativo a menos que você saiba o que está fazendo. Ele é fornecido SOMENTE para fins de pesquisa de segurança e, ao executá-lo, você deve presumir que resultará em perda catastrófica de dados e funcionalidade em todos os dispositivos iOS conectados ao seu iCloud. Não sou responsável por qualquer perda de dados ou funcionalidade do dispositivo. Não execute este aplicativo em qualquer dispositivo que você não possui ou em qualquer dispositivo vinculado a um iCloud que você não possui.**


## Como funciona
Quando um dispositivo HomeKit com uma string excepcionalmente grande (ex: 500.000 caracteres) é carregado no centro de controle, o dispositivo congela. Como esses dados são armazenados no iCloud, restaurar um dispositivo no qual o bug foi acionado e, em seguida, fazer login novamente na conta do iCloud associada acionará o bug novamente.

Este aplicativo renomeará todos os dispositivos HomeKit para a string contida em "ExploitString.swift", que por padrão é uma string que **Acionará o bug** no iOS 15.0 (possivelmente 14.8.1) e inferior. O bug AINDA pode ser acionado no iOS 15.2, no entanto, por meio de métodos como convites para casa. Mais detalhes sobre isso estão incluídos no link acima.


## Sobre
Como eu não tinha experiência anterior com o HomeKit, este aplicativo é apenas o projeto de amostra [Interacting with a Home Automation Network](https://developer.apple.com/documentation/homekit/interacting_with_a_home_automation_network) da Apple com modificações feitas para renomear todos os dispositivos HomeKit para o mesma string para demonstrar o bug.

