using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Databaselabweek1
{
    internal class overriding
    {
        static void Main(string[] args)
        {
            Bike A1 = new Bike();
            A1.MakeSound();

        }

        public class Bike
        {
            public virtual void MakeSound()
            {
                Console.WriteLine("Animal makes a sound");
            }
        }

        public class Tyre :Bike
        {
            public override void MakeSound()
            {
                Console.WriteLine("Rotates");
            }
        }

        public class Handle : Bike
        {
            public override void MakeSound()
            {
                Console.WriteLine("Turns");
            }
        }

        public class Engine : Bike
        {
            public override void MakeSound()
            {
                Console.WriteLine("Burns ");
            }
        }
   }
}
