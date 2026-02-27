using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Databaselabweek1
{
    internal class runtime
    {
        static void Main(string[] args)
        {
            Student student = new Student();
            student.st1(1, "name");
        }

        class Student
        {
            public int Id { get; set; }
            public string Name { get; set; }
            public string Description { get; set; }
            public string FirstName { get; set; }
            public string LastName { get; set; }

        public void st1(int id,string name)
            {
                Console.WriteLine("Id");
                Console.WriteLine("Name");
            }

            public void st2(int id, string name,string description)
            {
                Console.WriteLine("Id");
                Console.WriteLine("Name");
                Console.WriteLine($"{name} {description}");
            }

        }


    }
}
