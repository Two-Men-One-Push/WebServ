#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP

#include <stdexcept>
#include <string>

namespace HttpStatus {

enum Code {
	NoStatus = 0,

	/* 1xx - Informational */
	Continue = 100,
	SwitchingProtocols = 101,
	Processing = 102,
	EarlyHints = 103,

	/* 2xx - Successful */
	OK = 200,
	Created = 201,
	Accepted = 202,
	NonAuthoritativeInformation = 203,
	NoContent = 204,
	ResetContent = 205,
	PartialContent = 206,
	MultiStatus = 207,
	AlreadyReported = 208,
	IMUsed = 226,

	/* 3xx - Redirection */
	MultipleChoices = 300,
	MovedPermanently = 301,
	Found = 302,
	SeeOther = 303,
	NotModified = 304,
	UseProxy = 305,
	TemporaryRedirect = 307,
	PermanentRedirect = 308,

	/* 4xx - Client Error */
	BadRequest = 400,
	Unauthorized = 401,
	PaymentRequired = 402,
	Forbidden = 403,
	NotFound = 404,
	MethodNotAllowed = 405,
	NotAcceptable = 406,
	ProxyAuthenticationRequired = 407,
	RequestTimeout = 408,
	Conflict = 409,
	Gone = 410,
	LengthRequired = 411,
	PreconditionFailed = 412,
	ContentTooLarge = 413,
	PayloadTooLarge = 413,
	URITooLong = 414,
	UnsupportedMediaType = 415,
	RangeNotSatisfiable = 416,
	ExpectationFailed = 417,
	ImATeapot = 418,
	MisdirectedRequest = 421,
	UnprocessableContent = 422,
	UnprocessableEntity = 422,
	Locked = 423,
	FailedDependency = 424,
	TooEarly = 425,
	UpgradeRequired = 426,
	PreconditionRequired = 428,
	TooManyRequests = 429,
	RequestHeaderFieldsTooLarge = 431,
	UnavailableForLegalReasons = 451,

	/* 5xx - Server Error */
	InternalServerError = 500,
	NotImplemented = 501,
	BadGateway = 502,
	ServiceUnavailable = 503,
	GatewayTimeout = 504,
	HTTPVersionNotSupported = 505,
	VariantAlsoNegotiates = 506,
	InsufficientStorage = 507,
	LoopDetected = 508,
	NotExtended = 510,
	NetworkAuthenticationRequired = 511,
};

inline Code fromInt(int x) {
	switch (x) {

	// 1xx - Informational
	case 100:
		return Continue;
	case 101:
		return SwitchingProtocols;
	case 102:
		return Processing;
	case 103:
		return EarlyHints;

	// 2xx - Successful
	case 200:
		return OK;
	case 201:
		return Created;
	case 202:
		return Accepted;
	case 203:
		return NonAuthoritativeInformation;
	case 204:
		return NoContent;
	case 205:
		return ResetContent;
	case 206:
		return PartialContent;
	case 207:
		return MultiStatus;
	case 208:
		return AlreadyReported;
	case 226:
		return IMUsed;

	// 3xx - Redirection
	case 300:
		return MultipleChoices;
	case 301:
		return MovedPermanently;
	case 302:
		return Found;
	case 303:
		return SeeOther;
	case 304:
		return NotModified;
	case 305:
		return UseProxy;
	case 307:
		return TemporaryRedirect;
	case 308:
		return PermanentRedirect;

	// 4xx - Client Error
	case 400:
		return BadRequest;
	case 401:
		return Unauthorized;
	case 402:
		return PaymentRequired;
	case 403:
		return Forbidden;
	case 404:
		return NotFound;
	case 405:
		return MethodNotAllowed;
	case 406:
		return NotAcceptable;
	case 407:
		return ProxyAuthenticationRequired;
	case 408:
		return RequestTimeout;
	case 409:
		return Conflict;
	case 410:
		return Gone;
	case 411:
		return LengthRequired;
	case 412:
		return PreconditionFailed;
	case 413:
		return ContentTooLarge;
	case 414:
		return URITooLong;
	case 415:
		return UnsupportedMediaType;
	case 416:
		return RangeNotSatisfiable;
	case 417:
		return ExpectationFailed;
	case 418:
		return ImATeapot;
	case 421:
		return MisdirectedRequest;
	case 422:
		return UnprocessableContent;
	case 423:
		return Locked;
	case 424:
		return FailedDependency;
	case 425:
		return TooEarly;
	case 426:
		return UpgradeRequired;
	case 428:
		return PreconditionRequired;
	case 429:
		return TooManyRequests;
	case 431:
		return RequestHeaderFieldsTooLarge;
	case 451:
		return UnavailableForLegalReasons;

	// 5xx - Server Error
	case 500:
		return InternalServerError;
	case 501:
		return NotImplemented;
	case 502:
		return BadGateway;
	case 503:
		return ServiceUnavailable;
	case 504:
		return GatewayTimeout;
	case 505:
		return HTTPVersionNotSupported;
	case 506:
		return VariantAlsoNegotiates;
	case 507:
		return InsufficientStorage;
	case 508:
		return LoopDetected;
	case 510:
		return NotExtended;
	case 511:
		return NetworkAuthenticationRequired;
	default:
		throw std::invalid_argument("Not a valid status code");
	}
}

inline bool isInformational(int code) { return (code >= 100 && code < 200); }

inline bool isSuccessful(int code) { return (code >= 200 && code < 300); }

inline bool isRedirection(int code) { return (code >= 300 && code < 400); }

inline bool isClientError(int code) { return (code >= 400 && code < 500); }

inline bool isServerError(int code) { return (code >= 500 && code < 600); }

inline bool isError(int code) { return (code >= 400); }

inline std::string reasonPhrase(int code) {
	switch (code) {

	// 1xx - Informational
	case 100:
		return "Continue";
	case 101:
		return "Switching Protocols";
	case 102:
		return "Processing";
	case 103:
		return "Early Hints";

	// 2xx - Successful
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 202:
		return "Accepted";
	case 203:
		return "Non-Authoritative Information";
	case 204:
		return "No Content";
	case 205:
		return "Reset Content";
	case 206:
		return "Partial Content";
	case 207:
		return "Multi-Status";
	case 208:
		return "Already Reported";
	case 226:
		return "IM Used";

	// 3xx - Redirection
	case 300:
		return "Multiple Choices";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 303:
		return "See Other";
	case 304:
		return "Not Modified";
	case 305:
		return "Use Proxy";
	case 307:
		return "Temporary Redirect";
	case 308:
		return "Permanent Redirect";

	// 4xx - Client Error
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 402:
		return "Payment Required";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 406:
		return "Not Acceptable";
	case 407:
		return "Proxy Authentication Required";
	case 408:
		return "Request Timeout";
	case 409:
		return "Conflict";
	case 410:
		return "Gone";
	case 411:
		return "Length Required";
	case 412:
		return "Precondition Failed";
	case 413:
		return "Content Too Large";
	case 414:
		return "URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 416:
		return "Range Not Satisfiable";
	case 417:
		return "Expectation Failed";
	case 418:
		return "I'm a teapot";
	case 421:
		return "Misdirected Request";
	case 422:
		return "Unprocessable Content";
	case 423:
		return "Locked";
	case 424:
		return "Failed Dependency";
	case 425:
		return "Too Early";
	case 426:
		return "Upgrade Required";
	case 428:
		return "Precondition Required";
	case 429:
		return "Too Many Requests";
	case 431:
		return "Request Header Fields Too Large";
	case 451:
		return "Unavailable For Legal Reasons";

	// 5xx - Server Error
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Timeout";
	case 505:
		return "HTTP Version Not Supported";
	case 506:
		return "Variant Also Negotiates";
	case 507:
		return "Insufficient Storage";
	case 508:
		return "Loop Detected";
	case 510:
		return "Not Extended";
	case 511:
		return "Network Authentication Required";

	default:
		return std::string();
	}
}

} // namespace HttpStatus

#endif
