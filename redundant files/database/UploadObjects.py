import boto3

def upload_object(Object, xyVal, dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

    table = dynamodb.Table('Obejcts')
    response = table.put_item(
       Item={
            'Object': Object,
            'xyVal': xyVal, # this is a coordinate still. 
        }
    )
    return response

if __name__ == "__main__":
    print ("Uploading Objets...")
    inputObject = int(input("Object 1: "))
    inputCoord = int(input("Coordinate 1: "))
    upload_object(inputObject, inputCoord)