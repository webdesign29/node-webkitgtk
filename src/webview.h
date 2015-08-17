#ifndef WEBKITGTK_WEBVIEW_H
#define WEBKITGTK_WEBVIEW_H

#include "selfmessage.h"
#include <node.h>
#include <webkit2/webkit2.h>
#include <nan.h>
#include <gtk/gtkunixprint.h>
#include <map>

static const GDBusNodeInfo* introspection_data;

class WebView : public node::ObjectWrap {
public:
	static const int DOCUMENT_ERROR = -1;
	static const int DOCUMENT_AVAILABLE = 0;
	static const int DOCUMENT_LOADING = 1;
	static const int DOCUMENT_LOADED = 2;

	static void Init(v8::Handle<v8::Object>, v8::Handle<v8::Object>);
	static void Exit(void*);

#if WEBKIT_CHECK_VERSION(2,7,4)
	static const WebKitSnapshotOptions snapshot_options = WEBKIT_SNAPSHOT_OPTIONS_TRANSPARENT_BACKGROUND;
#else
	static const WebKitSnapshotOptions snapshot_options = WEBKIT_SNAPSHOT_OPTIONS_NONE;
#endif

	static void InspectorClosed(WebKitWebInspector*, gpointer);
	static void WindowClosed(GtkWidget*, gpointer);
	static gboolean Authenticate(WebKitWebView*, WebKitAuthenticationRequest*, gpointer);
	static void InitExtensions(WebKitWebContext*, gpointer);
	static gboolean DecidePolicy(WebKitWebView*, WebKitPolicyDecision*, WebKitPolicyDecisionType, gpointer);
	static void ResourceLoad(WebKitWebView*, WebKitWebResource*, WebKitURIRequest*, gpointer);
	static void ResourceResponse(WebKitWebResource*, gpointer);
	static void ResourceReceiveData(WebKitWebResource*, guint64, gpointer);
	static void Change(WebKitWebView*, WebKitLoadEvent, gpointer);
	static gboolean Fail(WebKitWebView*, WebKitLoadEvent, gchar*, GError*, gpointer);
	static gboolean ScriptDialog(WebKitWebView*, WebKitScriptDialog*, gpointer);
	static void PngFinished(GObject*, GAsyncResult*, gpointer);
	static cairo_status_t PngWrite(void*, const unsigned char*, unsigned int);
	static void RunFinished(GObject*, GAsyncResult*, gpointer);
	static void PrintFinished(WebKitPrintOperation*, gpointer);
	static void PrintFailed(WebKitPrintOperation*, gpointer, gpointer);

	static void handle_method_call(GDBusConnection*, const gchar*, const gchar*,
		const gchar*, const gchar*, GVariant*, GDBusMethodInvocation*, gpointer);
	static gboolean on_new_connection(GDBusServer*, GDBusConnection*, gpointer);

	void destroy();
	void unloaded();

	WebKitUserScript* userScript;
	WebKitUserStyleSheet* userStyleSheet;
private:
	static Nan::Persistent<v8::Function> constructor;
	WebView(v8::Handle<v8::Object>);
	~WebView();

	gchar* guid;
	GDBusServer* server;
	guint contextSignalId;

	gchar* uri = NULL;
	void updateUri(const gchar*);
	guint signalResourceResponse;

	int state;
	int authRetryCount;
	bool allowDialogs;
	bool offscreen;
	bool transparencySupport;

	bool userContent;
	bool waitFinish;


	WebKitWebView* view = NULL;
	GtkWidget* window = NULL;
	WebKitWebInspector* inspector = NULL;

	Nan::Callback* eventsCallback = NULL;
	char* eventName = NULL;

	Nan::Callback* pngCallback = NULL;
	Nan::Utf8String* pngFilename = NULL;

	Nan::Callback* printCallback = NULL;
	Nan::Utf8String* printUri = NULL;

	Nan::Callback* loadCallback = NULL;
	Nan::Callback* stopCallback = NULL;
	Nan::Callback* requestCallback = NULL;
	Nan::Callback* receiveDataCallback = NULL;
	Nan::Callback* responseCallback = NULL;
	Nan::Callback* policyCallback = NULL;
	Nan::Callback* authCallback = NULL;
	Nan::Callback* closeCallback = NULL;

	static NAN_METHOD(New);
	static NAN_METHOD(Load);
	static NAN_METHOD(Loop);
	static NAN_METHOD(Run);
	static NAN_METHOD(Png);
	static NAN_METHOD(Print);
	static NAN_METHOD(Stop);
	static NAN_METHOD(Destroy);
	static NAN_METHOD(Inspect);

	static NAN_GETTER(get_prop);
};

typedef std::map<char*, WebView*> ObjMap;
typedef std::pair<char*, WebView*> ObjMapPair;
static ObjMap instances;

#endif
