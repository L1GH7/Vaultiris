#import "ViewController.h"

#include "exploit/exploit.h"
#include "exploit/kernel_rw.h"

#include <pthread.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewDidAppear:(BOOL)animated {
    UIAlertController * disclaimerAndInstructions = [UIAlertController alertControllerWithTitle: @ "¡Bienvenido!"
                                     message: @"Pulsa el botón \"Ejecutar exploit (PoC)\" para comprobar si tu dispositivo es vulnerable. Funciona en las versiones 15.0-15.1.1 (NO EN 15.2 NI EN ADELANTE).\n\nDisclaimer: No me hago responsable si algo sale mal con tu dispositivo. Este exploit es hecho por potmdehex y está Open Source en GitHub. Lo único que hace es ejecutar el exploit cuando le das al botón. Si no estás conforme y no quieres arriesgarte, puedes salir de la aplicación. Si prefieres tomar el riesgo, go ahead y pulsa \"Continuar\"." preferredStyle: UIAlertControllerStyleAlert
                                    ];
      UIAlertAction * action = [UIAlertAction actionWithTitle: @ "Continuar"
                                style: UIAlertActionStyleDefault handler: ^ (UIAlertAction * _Nonnull action) {
                                  
                                }
                               ];
      [disclaimerAndInstructions addAction: action];
      [self presentViewController: disclaimerAndInstructions animated: true completion: nil];
}

int go(void)
{
    uint64_t kernel_base = 0;
    
    if (exploit_get_krw_and_kernel_base(&kernel_base) != 0)
    {
        UIAlertController * failedExploit = [UIAlertController alertControllerWithTitle: @ "❌ ¡Exploit fallido!"
                                         message: @"Puedes probar a reiniciar el dispositivo y volver a intentarlo. Si tras varios intentos ves esta misma pantalla, el exploit no es compatible." preferredStyle: UIAlertControllerStyleAlert
                                        ];
          UIAlertAction * action = [UIAlertAction actionWithTitle: @ "Entendido"
                                    style: UIAlertActionStyleDefault handler: ^ (UIAlertAction * _Nonnull action) {
                                      
                                    }
                                   ];
          [failedExploit addAction: action];
        return 1;
    }
    
    // test kernel r/w, read kernel base
    uint32_t mh_magic = kread32(kernel_base);
    if (mh_magic != 0xFEEDFACF)
    {
        printf("mh_magic != 0xFEEDFACF: %08X\n", mh_magic);
        return 1;
    }
    
    UIAlertController * yuhuuu = [UIAlertController alertControllerWithTitle: @ "✅ Exploit compatible"
                                     message: @"¡Fántastico! El exploit es compatible con tu dispositivo. Pero atento, NO ACTUALICES TU IPHONE/IPAD/IPOD. De lo contrario, ¡perderás acceso en el futuro a poder jailbreakear tu dispositivo!\n\nPro Tip: Si conectas el iPhone al Mac y lanzas Xcode, podrás ver información de debug que te dará una pista de la parte del kernel que ha conseguido leer (;" preferredStyle: UIAlertControllerStyleAlert
                                    ];
      UIAlertAction * action = [UIAlertAction actionWithTitle: @ "¡VAMONOOOS! 😃"
                                style: UIAlertActionStyleDefault handler: ^ (UIAlertAction * _Nonnull action) {
                                  
                                }
                               ];
      [yuhuuu addAction: action];
    
    printf("kread32(_kernel_base) success: %08X\n", mh_magic);
    
    printf("Done\n");
    
    return 0;
}

- (IBAction)runExploit:(id)sender {
    pthread_t pt;
    pthread_create(&pt, NULL, (void *(*)(void *))go, NULL);
}


@end
