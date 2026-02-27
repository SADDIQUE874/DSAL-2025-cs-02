using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Databaselabweek1
{
    internal class Program
    {
        static void Main(string[] args)
        {

            Student student = new Student(1, "Ali", "Saddique", "232w3", 345678);
            Console.WriteLine("Id: " + student.Id);
       }
      class Student
        {
            public int Id { get; set; }
            public string Name { get; set; }
            public string Surname { get; set; }
            public string Email { get; set; }
            public int PhoneNumber { get; set; }
            public Student(int id, string name, string surname, string email, int phoneNumber)
            {
                Id = id;
                Name = name;
                Surname = surname;
                Email = email;
                PhoneNumber = phoneNumber;
            }
        }
            
    }
}
