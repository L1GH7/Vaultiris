using System;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SecretBuster
{
    class Program
    {
        static void Main(string[] args)
        {
            string knownHash;
            string username;
            string email;
            string date;

            Console.Write("Enter the username: ");
            username = Console.ReadLine();

            Console.Write("Enter the email: ");
            email = Console.ReadLine();

            Console.Write("Enter the date (DDMM): ");
            date = Console.ReadLine();

            Console.Write("Enter the hash: ");
            knownHash = Console.ReadLine();

            Stopwatch sw = new Stopwatch();
            sw.Start();
            BruteForceSecret(username, email, date, knownHash, Int32.MinValue, Int32.MaxValue);
            sw.Stop();
            Console.WriteLine("Elapsed Time: {0:00}:{1:00}:{2:00}.{3:00}", sw.Elapsed.Hours, sw.Elapsed.Minutes, sw.Elapsed.Seconds, sw.Elapsed.Milliseconds / 10);

        }

        public static string GenerateRandomCode(int length, int seed)
        {
            Random random = new Random(seed);
            string text = "";
            string text2 = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
            int length2 = text2.Length;
            for (int i = 0; i < length; i++)
            {
                text += text2.Substring(random.Next(length2), 1);
            }
            return text;
        }

        public static string MD5Hash(string input)
        {
            HashAlgorithm arg_12_0 = new MD5CryptoServiceProvider();
            byte[] array = Encoding.UTF8.GetBytes(input);
            array = arg_12_0.ComputeHash(array);
            StringBuilder stringBuilder = new StringBuilder();
            byte[] array2 = array;
            for (int i = 0; i < array2.Length; i++)
            {
                byte b = array2[i];
                stringBuilder.Append(b.ToString("x2").ToLower());
            }
            return stringBuilder.ToString();
        }

        private static bool BruteForceSecret(string username, string email, string date, string knownHash, int startSeed = Int32.MinValue, int endSeed = Int32.MaxValue)
        {
            string useremail = username + email;
            long progress = 0;
            double totalLoops = (double)endSeed - (double)startSeed;
            Parallel.For(startSeed, endSeed, new ParallelOptions { MaxDegreeOfParallelism = 8 }, (int x, ParallelLoopState loopState) =>
            {
                string code = GenerateRandomCode(20, x);
                string calcHash = MD5Hash(useremail + code + date);
                //Console.WriteLine("seed: {0}\tcode: {1}\thash: {2}", x, code, calcHash);
                Interlocked.Increment(ref progress);
                if (progress % 1000000 == 0)
                {
                    Console.WriteLine("Progress: {0} / {1}", progress, totalLoops);
                    Console.WriteLine("Percent: %{0:0.00}", ((double)progress / totalLoops) * 100);
                }


                if (string.Compare(knownHash, calcHash) == 0)
                {
                    Console.WriteLine("^ Found!!");
                    Console.WriteLine("FOUND seed: {0}\tcode: {1}\thash: {2}", x, code, calcHash);
                    loopState.Stop();
                    return;
                }
            }
            );

            return false;
        }

    }
}
