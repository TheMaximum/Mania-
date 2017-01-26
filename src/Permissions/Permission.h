#ifndef PERMISSION_H_
#define PERMISSION_H_

//* Permission
/**
 * \brief Enum containing the permission levels.
 */
enum Permission
{
    Master = 4,
    Admin = 3,
    Operator = 2,
    User = 1
};

class Permissions
{
public:
    static std::string Text(Permission perm)
    {
        switch(perm)
        {
            case Permission::Master:
                return "Master";
            case Permission::Admin:
                return "Admin";
            case Permission::Operator:
                return "Operator";

            default:
                return "User";
        }
    }
};

#endif // PERMISSION_H_
