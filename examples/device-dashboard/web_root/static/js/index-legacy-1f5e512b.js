!function(){function t(e){return t="function"==typeof Symbol&&"symbol"==typeof Symbol.iterator?function(t){return typeof t}:function(t){return t&&"function"==typeof Symbol&&t.constructor===Symbol&&t!==Symbol.prototype?"symbol":typeof t},t(e)}function e(t,e){var n="undefined"!=typeof Symbol&&t[Symbol.iterator]||t["@@iterator"];if(!n){if(Array.isArray(t)||(n=function(t,e){if(!t)return;if("string"==typeof t)return r(t,e);var n=Object.prototype.toString.call(t).slice(8,-1);"Object"===n&&t.constructor&&(n=t.constructor.name);if("Map"===n||"Set"===n)return Array.from(t);if("Arguments"===n||/^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n))return r(t,e)}(t))||e&&t&&"number"==typeof t.length){n&&(t=n);var o=0,i=function(){};return{s:i,n:function(){return o>=t.length?{done:!0}:{done:!1,value:t[o++]}},e:function(t){throw t},f:i}}throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.")}var a,u=!0,s=!1;return{s:function(){n=n.call(t)},n:function(){var t=n.next();return u=t.done,t},e:function(t){s=!0,a=t},f:function(){try{u||null==n.return||n.return()}finally{if(s)throw a}}}}function r(t,e){(null==e||e>t.length)&&(e=t.length);for(var r=0,n=new Array(e);r<e;r++)n[r]=t[r];return n}function n(){"use strict";/*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/facebook/regenerator/blob/main/LICENSE */n=function(){return r};var e,r={},o=Object.prototype,i=o.hasOwnProperty,a=Object.defineProperty||function(t,e,r){t[e]=r.value},u="function"==typeof Symbol?Symbol:{},s=u.iterator||"@@iterator",c=u.asyncIterator||"@@asyncIterator",f=u.toStringTag||"@@toStringTag";function l(t,e,r){return Object.defineProperty(t,e,{value:r,enumerable:!0,configurable:!0,writable:!0}),t[e]}try{l({},"")}catch(e){l=function(t,e,r){return t[e]=r}}function h(t,e,r,n){var o=e&&e.prototype instanceof d?e:d,i=Object.create(o.prototype),u=new C(n||[]);return a(i,"_invoke",{value:I(t,r,u)}),i}function p(t,e,r){try{return{type:"normal",arg:t.call(e,r)}}catch(t){return{type:"throw",arg:t}}}r.wrap=h;var v="suspendedStart",y="suspendedYield",R="executing",m="completed",E={};function d(){}function O(){}function g(){}var T={};l(T,s,(function(){return this}));var b=Object.getPrototypeOf,_=b&&b(b(x([])));_&&_!==o&&i.call(_,s)&&(T=_);var S=g.prototype=d.prototype=Object.create(T);function w(t){["next","throw","return"].forEach((function(e){l(t,e,(function(t){return this._invoke(e,t)}))}))}function k(e,r){function n(o,a,u,s){var c=p(e[o],e,a);if("throw"!==c.type){var f=c.arg,l=f.value;return l&&"object"==t(l)&&i.call(l,"__await")?r.resolve(l.__await).then((function(t){n("next",t,u,s)}),(function(t){n("throw",t,u,s)})):r.resolve(l).then((function(t){f.value=t,u(f)}),(function(t){return n("throw",t,u,s)}))}s(c.arg)}var o;a(this,"_invoke",{value:function(t,e){function i(){return new r((function(r,o){n(t,e,r,o)}))}return o=o?o.then(i,i):i()}})}function I(t,r,n){var o=v;return function(i,a){if(o===R)throw new Error("Generator is already running");if(o===m){if("throw"===i)throw a;return{value:e,done:!0}}for(n.method=i,n.arg=a;;){var u=n.delegate;if(u){var s=A(u,n);if(s){if(s===E)continue;return s}}if("next"===n.method)n.sent=n._sent=n.arg;else if("throw"===n.method){if(o===v)throw o=m,n.arg;n.dispatchException(n.arg)}else"return"===n.method&&n.abrupt("return",n.arg);o=R;var c=p(t,r,n);if("normal"===c.type){if(o=n.done?m:y,c.arg===E)continue;return{value:c.arg,done:n.done}}"throw"===c.type&&(o=m,n.method="throw",n.arg=c.arg)}}}function A(t,r){var n=r.method,o=t.iterator[n];if(o===e)return r.delegate=null,"throw"===n&&t.iterator.return&&(r.method="return",r.arg=e,A(t,r),"throw"===r.method)||"return"!==n&&(r.method="throw",r.arg=new TypeError("The iterator does not provide a '"+n+"' method")),E;var i=p(o,t.iterator,r.arg);if("throw"===i.type)return r.method="throw",r.arg=i.arg,r.delegate=null,E;var a=i.arg;return a?a.done?(r[t.resultName]=a.value,r.next=t.nextLoc,"return"!==r.method&&(r.method="next",r.arg=e),r.delegate=null,E):a:(r.method="throw",r.arg=new TypeError("iterator result is not an object"),r.delegate=null,E)}function P(t){var e={tryLoc:t[0]};1 in t&&(e.catchLoc=t[1]),2 in t&&(e.finallyLoc=t[2],e.afterLoc=t[3]),this.tryEntries.push(e)}function L(t){var e=t.completion||{};e.type="normal",delete e.arg,t.completion=e}function C(t){this.tryEntries=[{tryLoc:"root"}],t.forEach(P,this),this.reset(!0)}function x(r){if(r||""===r){var n=r[s];if(n)return n.call(r);if("function"==typeof r.next)return r;if(!isNaN(r.length)){var o=-1,a=function t(){for(;++o<r.length;)if(i.call(r,o))return t.value=r[o],t.done=!1,t;return t.value=e,t.done=!0,t};return a.next=a}}throw new TypeError(t(r)+" is not iterable")}return O.prototype=g,a(S,"constructor",{value:g,configurable:!0}),a(g,"constructor",{value:O,configurable:!0}),O.displayName=l(g,f,"GeneratorFunction"),r.isGeneratorFunction=function(t){var e="function"==typeof t&&t.constructor;return!!e&&(e===O||"GeneratorFunction"===(e.displayName||e.name))},r.mark=function(t){return Object.setPrototypeOf?Object.setPrototypeOf(t,g):(t.__proto__=g,l(t,f,"GeneratorFunction")),t.prototype=Object.create(S),t},r.awrap=function(t){return{__await:t}},w(k.prototype),l(k.prototype,c,(function(){return this})),r.AsyncIterator=k,r.async=function(t,e,n,o,i){void 0===i&&(i=Promise);var a=new k(h(t,e,n,o),i);return r.isGeneratorFunction(e)?a:a.next().then((function(t){return t.done?t.value:a.next()}))},w(S),l(S,f,"Generator"),l(S,s,(function(){return this})),l(S,"toString",(function(){return"[object Generator]"})),r.keys=function(t){var e=Object(t),r=[];for(var n in e)r.push(n);return r.reverse(),function t(){for(;r.length;){var n=r.pop();if(n in e)return t.value=n,t.done=!1,t}return t.done=!0,t}},r.values=x,C.prototype={constructor:C,reset:function(t){if(this.prev=0,this.next=0,this.sent=this._sent=e,this.done=!1,this.delegate=null,this.method="next",this.arg=e,this.tryEntries.forEach(L),!t)for(var r in this)"t"===r.charAt(0)&&i.call(this,r)&&!isNaN(+r.slice(1))&&(this[r]=e)},stop:function(){this.done=!0;var t=this.tryEntries[0].completion;if("throw"===t.type)throw t.arg;return this.rval},dispatchException:function(t){if(this.done)throw t;var r=this;function n(n,o){return u.type="throw",u.arg=t,r.next=n,o&&(r.method="next",r.arg=e),!!o}for(var o=this.tryEntries.length-1;o>=0;--o){var a=this.tryEntries[o],u=a.completion;if("root"===a.tryLoc)return n("end");if(a.tryLoc<=this.prev){var s=i.call(a,"catchLoc"),c=i.call(a,"finallyLoc");if(s&&c){if(this.prev<a.catchLoc)return n(a.catchLoc,!0);if(this.prev<a.finallyLoc)return n(a.finallyLoc)}else if(s){if(this.prev<a.catchLoc)return n(a.catchLoc,!0)}else{if(!c)throw new Error("try statement without catch or finally");if(this.prev<a.finallyLoc)return n(a.finallyLoc)}}}},abrupt:function(t,e){for(var r=this.tryEntries.length-1;r>=0;--r){var n=this.tryEntries[r];if(n.tryLoc<=this.prev&&i.call(n,"finallyLoc")&&this.prev<n.finallyLoc){var o=n;break}}o&&("break"===t||"continue"===t)&&o.tryLoc<=e&&e<=o.finallyLoc&&(o=null);var a=o?o.completion:{};return a.type=t,a.arg=e,o?(this.method="next",this.next=o.finallyLoc,E):this.complete(a)},complete:function(t,e){if("throw"===t.type)throw t.arg;return"break"===t.type||"continue"===t.type?this.next=t.arg:"return"===t.type?(this.rval=this.arg=t.arg,this.method="return",this.next="end"):"normal"===t.type&&e&&(this.next=e),E},finish:function(t){for(var e=this.tryEntries.length-1;e>=0;--e){var r=this.tryEntries[e];if(r.finallyLoc===t)return this.complete(r.completion,r.afterLoc),L(r),E}},catch:function(t){for(var e=this.tryEntries.length-1;e>=0;--e){var r=this.tryEntries[e];if(r.tryLoc===t){var n=r.completion;if("throw"===n.type){var o=n.arg;L(r)}return o}}throw new Error("illegal catch attempt")},delegateYield:function(t,r,n){return this.delegate={iterator:x(t),resultName:r,nextLoc:n},"next"===this.method&&(this.arg=e),E}},r}function o(t,e,r,n,o,i,a){try{var u=t[i](a),s=u.value}catch(c){return void r(c)}u.done?e(s):Promise.resolve(s).then(n,o)}function i(t,e){var r=Object.keys(t);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(t);e&&(n=n.filter((function(e){return Object.getOwnPropertyDescriptor(t,e).enumerable}))),r.push.apply(r,n)}return r}function a(t){for(var e=1;e<arguments.length;e++){var r=null!=arguments[e]?arguments[e]:{};e%2?i(Object(r),!0).forEach((function(e){f(t,e,r[e])})):Object.getOwnPropertyDescriptors?Object.defineProperties(t,Object.getOwnPropertyDescriptors(r)):i(Object(r)).forEach((function(e){Object.defineProperty(t,e,Object.getOwnPropertyDescriptor(r,e))}))}return t}function u(t,e){if(!(t instanceof e))throw new TypeError("Cannot call a class as a function")}function s(t,e){for(var r=0;r<e.length;r++){var n=e[r];n.enumerable=n.enumerable||!1,n.configurable=!0,"value"in n&&(n.writable=!0),Object.defineProperty(t,l(n.key),n)}}function c(t,e,r){return e&&s(t.prototype,e),r&&s(t,r),Object.defineProperty(t,"prototype",{writable:!1}),t}function f(t,e,r){return(e=l(e))in t?Object.defineProperty(t,e,{value:r,enumerable:!0,configurable:!0,writable:!0}):t[e]=r,t}function l(e){var r=function(e,r){if("object"!==t(e)||null===e)return e;var n=e[Symbol.toPrimitive];if(void 0!==n){var o=n.call(e,r||"default");if("object"!==t(o))return o;throw new TypeError("@@toPrimitive must return a primitive value.")}return("string"===r?String:Number)(e)}(e,"string");return"symbol"===t(r)?r:String(r)}System.register(["./lodash-legacy-b10f79d7.js","./axios-legacy-45a42015.js","./index-legacy-4e9e76c8.js","./feedback-legacy-922e6bca.js"],(function(t,r){"use strict";var i,s,l,h,p,v,y,R;return{setters:[function(t){i=t.l},function(t){s=t.a,l=t.A},function(t){h=t.g,p=t.c,v=t.r,y=t.P},function(t){R=t.f}],execute:function(){var r={terminal:1,title:"多棱多智慧终端",version:"1.4.0",baseUrl:"".concat({}.VITE_APP_BASE_URL||"","/"),urlPrefix:"",timeout:1e4,withToken:!1,isTransformResponse:!1,isReturnDefaultResponse:!1},m=function(t){return t.JSON="application/json;charset=UTF-8",t.FORM_DATA="multipart/form-data;charset=UTF-8",t}(m||{}),E=function(t){return t.GET="GET",t.POST="POST",t}(E||{}),d=function(t){return t[t.SUCCESS=200]="SUCCESS",t[t.FAILED=300]="FAILED",t[t.PARAMS_VALID_ERROR=310]="PARAMS_VALID_ERROR",t[t.PARAMS_TYPE_ERROR=311]="PARAMS_TYPE_ERROR",t[t.REQUEST_METHOD_ERROR=312]="REQUEST_METHOD_ERROR",t[t.ASSERT_ARGUMENT_ERROR=313]="ASSERT_ARGUMENT_ERROR",t[t.ASSERT_MYBATIS_ERROR=314]="ASSERT_MYBATIS_ERROR",t[t.LOGIN_ACCOUNT_ERROR=330]="LOGIN_ACCOUNT_ERROR",t[t.LOGIN_DISABLE_ERROR=331]="LOGIN_DISABLE_ERROR",t[t.TOKEN_EMPTY=332]="TOKEN_EMPTY",t[t.TOKEN_INVALID=333]="TOKEN_INVALID",t[t.VERIFICATION_CODE_ERROR=334]="VERIFICATION_CODE_ERROR",t[t.NO_PERMISSTION=403]="NO_PERMISSTION",t[t.REQUEST_404_ERROR=404]="REQUEST_404_ERROR",t[t.SYSTEM_ERROR=500]="SYSTEM_ERROR",t}(d||{}),O=new Map,g=function(){function t(){u(this,t)}return c(t,[{key:"add",value:function(t){var e=t.url;this.remove(e),t.cancelToken=new s.CancelToken((function(t){O.has(e)||O.set(e,t)}))}},{key:"remove",value:function(t){if(O.has(t)){var e=O.get(t);e&&e(t),O.delete(t)}}}],[{key:"createInstance",value:function(){var e;return null!==(e=this.instance)&&void 0!==e?e:this.instance=new t}}]),t}();f(g,"instance",void 0);var T=g.createInstance(),b=function(){function t(e){u(this,t),f(this,"axiosInstance",void 0),f(this,"config",void 0),f(this,"options",void 0),this.config=e,this.options=e.requestOptions,this.axiosInstance=s.create(e),this.setupInterceptors()}return c(t,[{key:"getAxiosInstance",value:function(){return this.axiosInstance}},{key:"setupInterceptors",value:function(){var t=this;if(this.config.axiosHooks){var e=this.config.axiosHooks,r=e.requestInterceptorsHook,n=e.requestInterceptorsCatchHook,o=e.responseInterceptorsHook,a=e.responseInterceptorsCatchHook;this.axiosInstance.interceptors.request.use((function(e){return t.addCancelToken(e),i.isFunction(r)&&(e=r(e)),e}),(function(t){return i.isFunction(n)&&n(t),t})),this.axiosInstance.interceptors.response.use((function(e){return t.removeCancelToken(e.config.url),i.isFunction(o)&&(e=o(e)),e}),(function(e){var r;(i.isFunction(a)&&a(e),e.code!=l.ERR_CANCELED)&&t.removeCancelToken(null===(r=e.config)||void 0===r?void 0:r.url);return e.code==l.ECONNABORTED||e.code==l.ERR_NETWORK?new Promise((function(t){return setTimeout(t,500)})).then((function(){return t.retryRequest(e)})):Promise.reject(e)}))}}},{key:"addCancelToken",value:function(t){!t.requestOptions.ignoreCancelToken&&T.add(t)}},{key:"removeCancelToken",value:function(t){T.remove(t)}},{key:"retryRequest",value:function(t){var e,r,n=t.config,o=n.requestOptions,i=o.retryCount;return o.isOpenRetry&&(null===(e=n.method)||void 0===e?void 0:e.toUpperCase())!=E.POST?(n.retryCount=null!==(r=n.retryCount)&&void 0!==r?r:0,n.retryCount>=i?Promise.reject(t):(n.retryCount++,this.axiosInstance.request(n))):Promise.reject(t)}},{key:"get",value:function(t,e){return this.request(a(a({},t),{},{method:E.GET}),e)}},{key:"post",value:function(t,e){return this.request(a(a({},t),{},{method:E.POST}),e)}},{key:"request",value:function(t,e){var r=this,n=i.merge({},this.options,e),o=a(a({},i.cloneDeep(t)),{},{requestOptions:n}),u=n.urlPrefix;return u&&(o.url="".concat(u).concat(t.url)),new Promise((function(t,e){r.axiosInstance.request(o).then((function(e){t(e)})).catch((function(t){e(t)}))}))}}]),t}(),_={requestInterceptorsHook:function(t){var e,r=t.requestOptions,n=r.withToken,o=r.isParamsToData,i=t.params||{},a=t.headers||{};if(n){var u=h();a["like-admin"]=u}return o&&!Reflect.has(t,"data")&&(null===(e=t.method)||void 0===e?void 0:e.toUpperCase())===E.POST&&(t.data=i,t.params={}),t.headers=a,t},requestInterceptorsCatchHook:function(t){return t},responseInterceptorsHook:function(t){return(e=n().mark((function e(){var r,o,i,a,u,s,c;return n().wrap((function(e){for(;;)switch(e.prev=e.next){case 0:if(r=t.config.requestOptions,o=r.isTransformResponse,!r.isReturnDefaultResponse){e.next=3;break}return e.abrupt("return",t);case 3:if(o){e.next=5;break}return e.abrupt("return",t.data);case 5:i=t.data,a=i.code,u=i.data,s=i.show,c=i.msg,e.t0=a,e.next=e.t0===d.SUCCESS?9:e.t0===d.PARAMS_TYPE_ERROR||e.t0===d.PARAMS_VALID_ERROR||e.t0===d.REQUEST_METHOD_ERROR||e.t0===d.ASSERT_ARGUMENT_ERROR||e.t0===d.ASSERT_MYBATIS_ERROR||e.t0===d.LOGIN_ACCOUNT_ERROR||e.t0===d.LOGIN_DISABLE_ERROR||e.t0===d.NO_PERMISSTION||e.t0===d.FAILED||e.t0===d.SYSTEM_ERROR||e.t0===d.VERIFICATION_CODE_ERROR?11:e.t0===d.TOKEN_INVALID||e.t0===d.TOKEN_EMPTY?13:16;break;case 9:return s&&c&&R.msgSuccess(c),e.abrupt("return",u);case 11:return c&&R.msgError(c),e.abrupt("return",Promise.reject(u));case 13:return p(),v.push(y.LOGIN),e.abrupt("return",Promise.reject());case 16:return e.abrupt("return",u);case 17:case"end":return e.stop()}}),e)})),function(){var t=this,r=arguments;return new Promise((function(n,i){var a=e.apply(t,r);function u(t){o(a,n,i,u,s,"next",t)}function s(t){o(a,n,i,u,s,"throw",t)}u(void 0)}))})();var e},responseInterceptorsCatchHook:function(t){return t.code!==l.ERR_CANCELED&&t.message&&R.msgError(t.message),Promise.reject(t)}},S={timeout:r.timeout,baseURL:r.baseUrl,headers:{"Content-Type":m.JSON,version:r.version},axiosHooks:_,requestOptions:{isParamsToData:!0,isReturnDefaultResponse:r.isReturnDefaultResponse,isTransformResponse:r.isTransformResponse,urlPrefix:r.urlPrefix,ignoreCancelToken:!1,withToken:r.withToken,isOpenRetry:!0,retryCount:2}};var w,k=new b(i.merge(S,w||{})),I=(t("r",k),new Map);I.set("温度","℃"),I.set("温差","℃"),I.set("电压","V"),I.set("压差","mV"),I.set("电流","A"),I.set("有功功率","kW"),I.set("功率设定","kW"),I.set("储能功率","kW"),I.set("关口功率","kW"),I.set("光伏功率","kW"),I.set("负载功率","kW"),I.set("变压器功率","kW"),I.set("充电关口限制","kW"),I.set("放电关口限制","kW"),I.set("充电变压器限制","kW"),I.set("放电变压器限制","kW"),I.set("无功功率","kVar"),I.set("频率","Hz"),I.set("SOC","%"),I.set("soc","%"),I.set("SOH","%"),I.set("soh","%"),I.set("电阻","kΩ"),I.set("R+","kΩ"),I.set("R-","kΩ"),I.set("转速","RPM"),I.set("压力","Bar"),I.set("电能","kWh"),I.set("浓度","ppm"),I.set("CO","ppm"),I.set("co","ppm"),I.set("VOC","mV"),I.set("voc","mV");var A=function(t){var r,n="",o=e(I.keys());try{for(o.s();!(r=o.n()).done;){var i=r.value;if(t.endsWith(i)){n=I.get(i);break}}}catch(a){o.e(a)}finally{o.f()}return n};t("j",(function(t){var e=[];for(var r in t){var n={};n.name=r,n.value=t[r],n.unit=A(r),e.push(n)}return e}))}}}))}();
