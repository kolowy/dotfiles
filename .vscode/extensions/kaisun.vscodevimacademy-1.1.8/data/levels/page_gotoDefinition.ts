let var1 = 1;
let var2 = false;
let var3 = [var1, 1, 2, 3, 4];
let var4 = 'random vars';

const CLONE_SYMBOLS_FLAG = 4

// defs here
function insertBefore(stuff?: string) { }
function create(stuff?: string) { }
function debounce(stuff?: string) { }

const reIsDeepProp = /\.|\[(?:[^[\]]*|(["'])(?:(?!\1)[^\\]|\\.)*?\1)\]/
const reIsPlainProp = /^\w*$/


function isKey(value, object) {
    if (Array.isArray(value)) {
        return false
    }
    const type = typeof value
    if (type === 'number' || type === 'boolean' || value == null) {
        return true
    }
    return reIsPlainProp.test(value) || !reIsDeepProp.test(value) ||
        (object != null && value in Object(object))
}


function castPath(value, object) {
    if (Array.isArray(value)) {
        return value
    }
    return isKey(value, object) ? [value] : stringToPath(value)
}

function stringToPath(string) {
    create();
    return string;
}


// refs here
(function () {
    insertBefore();
    debounce();
    create();
    castPath(var1, var2);
    debounce();
    castPath(var1, var2);
})
