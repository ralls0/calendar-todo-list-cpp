# GoogleOAuth2

This program covers how to use Google to authorize an application.

Before start working with this code, you need to  register a new project in the service provider.
This step is necessary because you have to request the client credentials to identify your application. The following steps are based on Google API Console.

## Creating a new project

To start the project registration, access [Google API Console](https://console.cloud.google.com/apis/dashboard), then, press "Create Project".
A “New Project” dialog appears. Type the name of your project and click the "Create" button. Now you are ready to manage your new project.
Your application is registered, the dashboard is updated.

### Creating the credentials

You have to get credentials to identify the application.
Go to the "Credentials" section in the dashboard and press the "Create credentials" button. You need to choose which type of credentials you need.
Choose "OAuth client ID".

Now, you can configure Application type, Name, and under "Restrictions" there is an important field: the "Authorized redirect URIs".

f the user chooses to allow your application to manage his data.
The application has to receive an "access token" to be used during authenticated calls.
The "access token" can be received in several ways, but the most common way is to receive a call to your web server with the "access token".
In web applications, a server path to handle the notification is enough.
A desktop application need to fake it using a local browser.

So, to make sure the application works write `http://127.0.0.1:8080/`.

> NOTE: actually `QtNetworkAuth` does not allow you to set up the redirect URI,
> in fact, it is used by default `http://127.0.0.1`. In contrast, you can choose a different port.

End the process by pressing the "Create" button and you will see a new credential in the list of credentials with an "Edit", "Delete" and "Download" buttons at the right.
Click the download button and...
Finally, you get a JSON file ready to parse!

e.g.

```json
{
    "web": {
        "client_id": "xxxx-xxxxspi.apps.googleusercontent.com",
        "project_id": "project-id-xxxxx",
        "auth_uri": "https://accounts.google.com/o/oauth2/auth",
        "token_uri": "https://oauth2.googleapis.com/token",
        "auth_provider_x509_cert_url": "https://www.googleapis.com/oauth2/v1/certs",
        "client_secret": "xxxx-xxxx",
        "redirect_uris": [
            "http://127.0.0.1:8080/"
        ]
    }
}
```

Save this file under `cmake-build-debug` directory as `client-setup.json`.