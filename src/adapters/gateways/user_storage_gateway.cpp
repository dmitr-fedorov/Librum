#include "user_storage_gateway.hpp"
#include <QJsonObject>


using namespace domain::models;

namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* userStorageAccess) :
    m_userStorageAccess(userStorageAccess)
{
    connect(m_userStorageAccess, &IUserStorageAccess::userReady, this,
            &UserStorageGateway::proccessUserData);

    connect(m_userStorageAccess, &IUserStorageAccess::gettingUserFailed, this,
            &UserStorageGateway::reportFailureGettingUser);
}

void UserStorageGateway::getUser(const QString& authToken)
{
    m_userStorageAccess->getUser(authToken);
}

void UserStorageGateway::changeFirstName(const QString& authToken,
                                         const QString& newFirstName)
{
    m_userStorageAccess->changeFirstName(authToken, newFirstName);
}

void UserStorageGateway::changeLastName(const QString& authToken,
                                        const QString& newLastName)
{
    m_userStorageAccess->changeLastName(authToken, newLastName);
}

void UserStorageGateway::changeEmail(const QString& authToken,
                                     const QString& newEmail)
{
    m_userStorageAccess->changeEmail(authToken, newEmail);
}

void UserStorageGateway::changeProfilePicture(const QString& authToken,
                                              const QImage& newPicture)
{
    m_userStorageAccess->changeProfilePicture(authToken, newPicture);
}

void UserStorageGateway::proccessUserData(const QString& firstName,
                                          const QString& lastName,
                                          const QString& email,
                                          const QJsonArray& tags)
{
    User user(firstName, lastName, email);

    for(const auto& tag : tags)
    {
        auto jsonTagObject = tag.toObject();
        renameJsonObjectKey(jsonTagObject, "guid", "uuid");

        auto tagToAdd = Tag::fromJson(jsonTagObject);
        user.addTag(tagToAdd);
    }

    emit finishedGettingUser(user, true);
}

void UserStorageGateway::reportFailureGettingUser()
{
    User emptyUser;
    emit finishedGettingUser(emptyUser, false);
}

void UserStorageGateway::renameJsonObjectKey(QJsonObject& jsonObject,
                                             const QString& oldKeyName,
                                             const QString& newKeyName)
{
    auto temp = jsonObject[oldKeyName].toString();
    jsonObject.remove(oldKeyName);
    jsonObject.insert(newKeyName, temp);
}

}  // namespace adapters::gateways