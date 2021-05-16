const runWorker = require('../build/Release/napi-asyncworker-example-native');

let result = runWorker.runSimpleAsyncWorker({ runTime: 2 }, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '" + result + "'.");

function AsyncWorkerCompletion(err, result) {
    if (err) {
        console.log("SimpleAsyncWorker returned an error: ", err);
    } else {
        console.log("SimpleAsyncWorker returned '" + result + "'.");
    }
};
